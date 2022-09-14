#include "JobSystem.h"

#include "Native/Thread.h"
#include <vectormath/stdmath_extensions.h>

constexpr int32_t JOB_SYSTEM_MAX_WORKERS    = 16;
constexpr int32_t JOB_SYSTEM_MAX_JOBS       = 32;

/// This data structure only use for JobSystem
struct Job
{
    JobFunc*        func;
    void*           data;
};

/// The job system for parallel computing. The workers donot steal job.
/// The JobSystem must be alive long enough before worker threads done,
/// should allocate JobSystem's memory in global/static scope, main() function, or heap.
struct JobSystemState
{
    Thread          workerThreads[JOB_SYSTEM_MAX_WORKERS]               = {};   // Workers' thread description

    Job             jobs[JOB_SYSTEM_MAX_WORKERS][JOB_SYSTEM_MAX_JOBS]   = {};   // Jobs ring buffer
    int32_t         queueIndex[JOB_SYSTEM_MAX_WORKERS]                  = {};   // Safe index to queue a new job
    int32_t         runningIndex[JOB_SYSTEM_MAX_WORKERS]                = {};   // The current running job
    int32_t         currentWorkerIndex                                  = 0;    // Find out which worker receive the job

    ThreadMutex     mutex                                               = {};
    ThreadSignal    idleSignal                                          = {};
    ThreadSignal    queueSignal                                         = {};

    int32_t         workerCount                                         = 0;
    int32_t         idleWorkerCount                                     = 0;

    volatile bool   running                                             = false;

    static int ThreadFunc(void* data)
    {
        JobSystemState* jobSystem = (JobSystemState*)data;

        jobSystem->MainLoop();

        return 0;
    }

    void Create(int32_t requestWorkers)
    {
        const int threadCount = max(1, ThreadSystem::GetCpuCores());
        const int workerCount = requestWorkers <= 0 ? threadCount : min(threadCount, min(requestWorkers, JOB_SYSTEM_MAX_WORKERS));

        this->mutex.Create();
        this->idleSignal.Create();
        this->queueSignal.Create();

        this->running = true;

        this->idleWorkerCount = 0;

        this->workerCount = workerCount;
        for (int i = 0; i < workerCount; i++)
        {
            this->queueIndex[i] = 0;
            this->runningIndex[i] = 0;
            this->workerThreads[i].Start(ThreadFunc, this);
        }

        this->WaitIdle();
    }

    void Destroy()
    {
        this->mutex.Lock();
        this->running = false;
        this->mutex.Unlock();

        this->queueSignal.Broadcast();

        for (int i = 0, n = this->workerCount; i < n; i++)
        {
            this->queueIndex[i] = 0;
            this->runningIndex[i] = 0;
            this->workerThreads[i].Wait();
        }

        this->queueSignal.Destroy();
        this->idleSignal.Destroy();
        this->mutex.Destroy();
    }

    bool IsIdle_NonLocking()
    {
        bool allWorkersIdle = true;
        for (int i = 0, n = this->workerCount; i < n; i++)
        {
            if (this->queueIndex[i] != this->runningIndex[i])
            {
                allWorkersIdle = false;
                break;
            }
        }

        return (allWorkersIdle && (this->idleWorkerCount == this->workerCount)) || !this->running;
    }

    bool IsIdle()
    {
        this->mutex.Lock();

        const bool isIdle = IsIdle_NonLocking();

        this->mutex.Unlock();
        return isIdle;
    }

    void WaitIdle()
    {
        this->mutex.Lock();

        while (!IsIdle_NonLocking())
        {
            this->idleSignal.Wait(this->mutex);
        }

        this->mutex.Unlock();
    }

    void QueueJob(JobFunc* func, void* items)
    {
        this->mutex.Lock();

        const int workerIndex = this->currentWorkerIndex;

        Job* jobs = this->jobs[workerIndex];
        int* queueIndex = &this->queueIndex[workerIndex];

        jobs[*queueIndex] = Job{ func, items };
        *queueIndex = (*queueIndex + 1) % JOB_SYSTEM_MAX_JOBS;

        this->currentWorkerIndex = (workerIndex + 1) % this->workerCount;

        this->mutex.Unlock();

        this->queueSignal.Broadcast();
    }

    void MainLoop()
    {
        this->mutex.Lock();

        const int workerIndex = this->currentWorkerIndex;

        Job* jobs = this->jobs[workerIndex];
        int* queueIndex = &this->queueIndex[workerIndex];
        int* runningIndex = &this->runningIndex[workerIndex];

        this->currentWorkerIndex = (this->currentWorkerIndex + 1) % this->workerCount;
        this->mutex.Unlock();

        while (this->running)
        {
            this->mutex.Lock();

            this->idleWorkerCount++;
            while (this->running && *queueIndex == *runningIndex)
            {
                this->idleSignal.Broadcast();
                this->queueSignal.Wait(this->mutex);
            }
            this->idleWorkerCount--;
            this->mutex.Unlock();

            while (*queueIndex != *runningIndex)
            {
                this->mutex.Lock();
                Job job = jobs[*runningIndex];

                *runningIndex = (*runningIndex + 1) % JOB_SYSTEM_MAX_JOBS;

                this->mutex.Unlock();

                job.func(job.data);
            }
        }

        this->mutex.Lock();

        this->idleWorkerCount++;
        this->idleSignal.Broadcast();

        this->mutex.Unlock();
    }
};

static JobSystemState gJobSystem;

void JobSystem::Setup(void)
{
    gJobSystem.Create(-1);
}

void JobSystem::Shutdown(void)
{
    gJobSystem.Destroy();
}

bool JobSystem::IsIdle(void)
{
    return gJobSystem.IsIdle();
}

void JobSystem::WaitIdle()
{
    gJobSystem.WaitIdle();
}

void JobSystem::Queue(JobFunc* func, void* items)
{
    gJobSystem.QueueJob(func, items);
}
