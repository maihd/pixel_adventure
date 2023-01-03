#include "Application.h"

#include <float.h>
#include <stdio.h>

#ifndef NDEBUG
#   define RUN_UNIT_TESTS
#   define UNIT_TESTS_MAIN RunAllUnitTests
#   include "../unit_tests/test_framework.h"
#endif

#include <SDL2/SDL_main.h>
#include "Runtime.h"

int main(int argc, char* argv[])
{
    // Setup core runtime: memory, containers, functionals
    Runtime::Setup();

#ifndef NDEBUG
    int runTestsStatus = RunAllUnitTests(argc, argv);
    if (runTestsStatus != 0)
    {
        return runTestsStatus;
    }
#endif
    
    int appResult = ApplicationMain(argc, argv);

    // Shutdown core runtime
    // Make sure no memory allocations occur, memory must not leaked
    Runtime::Shutdown();
    
    return appResult;
}

//! LEAVE AN EMPTY LINE HERE, REQUIRE BY GCC/G++
