#include "Runtime.h"
#include "Native/Memory.h"

#include <rmem.h>

bool Runtime::Setup(void)
{
#ifndef NDEBUG
    rmemInit(nullptr);
#endif
    
    return true;
}

void Runtime::Shutdown(void)
{
#ifndef NDEBUG
    rmemShutDown();
#endif
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
