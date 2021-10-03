#include "Application.h"

#include <float.h>
#include <stdio.h>

#ifndef NDEBUG
#   define RUN_UNIT_TESTS
#   define UNIT_TESTS_MAIN RunAllUnitTests
#   include "../unit_tests/test_framework.h"
#endif

#if defined(_WIN32) && defined(SUBSYSTEM_WINDOWS)
#include <Windows.h>
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nShowCmd)
#else
int main(const int argc, const char* argv[])
#endif
{
#ifndef NDEBUG
    int runTestsStatus = RunAllUnitTests(argc, argv);
    if (runTestsStatus != 0)
    {
        return runTestsStatus;
    }
#endif

#if defined(_WIN32) && defined(SUBSYSTEM_WINDOWS)
    int         argc    = 0;
    const char* argv[]  = { "" };
#endif
    
    return ApplicationMain(argc, argv);
}
