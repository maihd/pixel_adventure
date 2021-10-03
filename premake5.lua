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
    
    includedirs {
        path.join(ROOT_DIR, "src"),
        path.join(ROOT_DIR, "3rd_party"),
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
        
        filter {}
    end

    filter {}
end
