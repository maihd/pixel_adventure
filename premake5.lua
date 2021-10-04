local ROOT_DIR = path.getabsolute(".")
local BUILD_DIR = path.join(ROOT_DIR, "projects")

local ENV = require("premake5.env")

local function filedirs(dirs)
    if type(dirs) == "string" then
        files {
            path.join(ROOT_DIR, dirs, "*.h"),
            path.join(ROOT_DIR, dirs, "*.c"),
            path.join(ROOT_DIR, dirs, "*.hpp"),
            path.join(ROOT_DIR, dirs, "*.cpp"),
            path.join(ROOT_DIR, dirs, "*.natvis"),
        }
    elseif type(dirs) == "table" then
        for _, dir in ipairs(dirs) do
            filedirs(dir)
        end
    end
end

workspace("PixelAdventure." .. string.upper(_ACTION))
do
    language "C++"
    location (path.join(BUILD_DIR, _ACTION))

    configurations { "Debug", "Release" }
    platforms { "x32", "x64" }

    flags {
        "NoPCH",
        "NoRuntimeChecks",
        "ShadowedVariables",
        "LinkTimeOptimization",

        --"FatalWarnings",
        --"FatalLinkWarnings",
        --"FatalCompileWarnings",
    }

    cppdialect "C++11"
    staticruntime "On"
    omitframepointer "On"

    rtti "On"
    exceptionhandling "Off"

    filter { "configurations:*Debug" }
    do
        optimize "Off"
        defines {
            --"_DEBUG",
            "SUBSYSTEM_CONSOLE"
        }

        filter {}
    end

    filter { "configurations:*Release" }
    do
        optimize "Full"
        defines {
            "NDEBUG",
            "SUBSYSTEM_WINDOWS"
        }

        filter {}
    end

    filter {}
end

project("PixelAdventure." .. string.upper(_ACTION))
do
    kind "ConsoleApp"

    defines {
    }

    links {
        "SDL2",
        "SDL2main"
    }
    
    includedirs {
        path.join(ROOT_DIR, "src"),
        path.join(ROOT_DIR, "3rd_party"),
        path.join(ROOT_DIR, "3rd_party/glad/include"),
        path.join(ROOT_DIR, "3rd_party/LDtkLoader/include"),
    }

    files {
        path.join(ROOT_DIR, "unit_tests/test_framework.h"),
    }

    filedirs {
        "src",
        "src/Container",
        "src/Graphics",
        "src/Framework",
        "src/Game",
        "src/Math",
        "src/Misc",
        "src/Text",

        "src/Native",
        
        "3rd_party/imgui",
        "3rd_party/glad/src",
        "3rd_party/LDtkLoader/src",

        "src/ThirdPartyImpl",
    }

    filter "configurations:*Debug"
    do
        filedirs {
            "unit_tests/cases"
        }

        filter {}
    end

    filter "action:vs*"
    do
        local SDL_PATH = path.join(ROOT_DIR, "3rd_party/SDL2-devel-2.0.16-VC")

        includedirs {
            path.join(SDL_PATH, "include")
        }

        filedirs { 
            "src/Native/Win32",
            "src/ThirdPartyImpl/Win32",
        }

        filter "configurations:*Debug"
        do
        end

        filter "configurations:*Release"
        do
            linkoptions "/SUBSYSTEM:WINDOWS"
        end

        filter "platforms:x32"
        do
            libdirs {
                path.join(SDL_PATH, "lib/x86")
            }

            postbuildcommands {
                "xcopy \"" .. path.join(SDL_PATH, "lib/x86/SDL2.dll") .. "\" \"$(OutDir)\" /D /E /I /F /Y"
            }
        end

        filter "platforms:x64"
        do
            libdirs {
                path.join(SDL_PATH, "lib/x64")
            }

            postbuildcommands {
                "xcopy \"" .. path.join(SDL_PATH, "lib/x64/SDL2.dll") .. "\" \"$(OutDir)\" /D /E /I /F /Y"
            }
        end
        
        filter {}
    end

    filter {}
end
