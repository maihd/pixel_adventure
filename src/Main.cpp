#include "Application.h"

#include <float.h>
#include <stdio.h>

#ifndef NDEBUG
#   define RUN_UNIT_TESTS
#   define UNIT_TESTS_MAIN RunAllUnitTests
#   include "../unit_tests/test_framework.h"
#endif

#include <SDL2/SDL_main.h>

int main(int argc, char* argv[])
{
#ifndef NDEBUG
    int runTestsStatus = RunAllUnitTests(argc, argv);
    if (runTestsStatus != 0)
    {
        return runTestsStatus;
    }
#endif
    
    return ApplicationMain(argc, argv);
}
