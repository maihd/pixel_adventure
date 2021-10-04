#include <time.h>
#include <math.h>
#include <stdint.h>

#include "Timer.h"
#include "Native/Thread.h"

static float    gDeltaTime          = 0.0f;
static float    gTotalTime          = 0.0f;
static float    gTimeScale          = 1.0f;
static float    gFrameRate          = 60.0f;

static int64_t  gTimerFrequency     = 0;
static int64_t  gTimerTicks         = 0;
static int64_t  gTimerInterval      = 0;
static double   gInvTimerFrequency  = 0.0;

void Timer::NewFrame(void)
{
    if (gTimerTicks == 0)
    {
        gTimerTicks = ThreadSystem::GetCpuTicks();
    }

    gTimerFrequency     = ThreadSystem::GetCpuFrequency();
    gTimerInterval      = gTimerFrequency / (int64_t)roundf(gFrameRate);
    gInvTimerFrequency  = 1.0 / (double)gTimerFrequency;
}

void Timer::EndFrame(void)
{
    int64_t currentTicks = ThreadSystem::GetCpuTicks();
    int64_t elapsedTicks = currentTicks - gTimerTicks;

    if (elapsedTicks < gTimerInterval)
    {
        double  sleepSeconds        = (double)(gTimerInterval - elapsedTicks) * gInvTimerFrequency;
        int64_t sleepMicroSeconds   = (int64_t)(sleepSeconds * 1000 * 1000);
        ThreadSystem::MicroSleep(sleepMicroSeconds);

        elapsedTicks = gTimerInterval;
    }

    gDeltaTime = (float)((double)elapsedTicks * gInvTimerFrequency);
    gTotalTime = gTotalTime + gTotalTime;

    gTimerTicks += elapsedTicks;
}

float Timer::GetDeltaTime(void)
{
    return gDeltaTime;
}

float Timer::GetTotalTime(void)
{
    return gTotalTime;
}

void Timer::SetFrameRate(float frameRate)
{
    gFrameRate = frameRate;
}

float Timer::GetFrameRate(void)
{
    return gFrameRate;
}

void Timer::SetTimeScale(float timeScale)
{
    gTimeScale = timeScale;
}

float Timer::GetTimeScale(void)
{
    return gTimeScale;
}
