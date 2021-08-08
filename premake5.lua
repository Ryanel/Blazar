-- premake5.lua

-- Setup the build configuration
build_directory = 'build'
outputdir="%{cfg.buildcfg}-%{cfg.system}"

editorintegration "On"     -- Integrate with VS
editAndContinue "Off"      -- Causes weird errors

workspace "Blazar"
    architecture "x64"
    configurations { "Debug", "Release", "Distribution" }
    location (build_directory)
    flags { 'MultiProcessorCompile' }

    filter "system:windows"
        defines("_ITERATOR_DEBUG_LEVEL=0") -- Prevent excessive stl container stuff
    
output_dir="%{cfg.buildcfg}-%{cfg.system}"

IncludeDir= {}
library_includes={}
library_links={}

include "src/Vendor/vendor.lua"

project "Blazar"
    location "build/Blazar/"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/Intermediate/" .. outputdir .. "/%{prj.name}")

    pchheader "bzpch.h"
    pchsource "src/Blazar/bzpch.cpp"

    links(library_links)
    includedirs("src/%{prj.name}/")
    includedirs(library_includes)

    files
    {
        "src/%{prj.name}/**.h",
        "src/%{prj.name}/**.cpp",
        "build/reflection_types.cpp"
    }

    filter "configurations:Debug"
        symbols "On"
        runtime "Debug"
        defines ({"BLAZAR_DEBUG"})
        defines("TRACY_ENABLE")

    filter "configurations:Release"
        defines ({"BLAZAR_RELEASE"})
        optimize "On"
        runtime "Release"
        defines("TRACY_ENABLE")

    filter "configurations:Distrobution"
        defines ({"BLAZAR_DIST", "BLAZAR_RELEASE"})
        optimize "Full"
        runtime "Release"

    filter "system:windows"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "BLAZAR_PLATFORM_WINDOWS",
            "BLAZAR_BUILD_STATIC",
            "GLFW_INCLUDE_NONE",
        }

        disablewarnings({"26812", "26495", "26451", "26498", "6201", "6285", "26437"})

    filter "system:linux"
        defines
        {
            "BLAZAR_PLATFORM_LINUX",
            "BLAZAR_BUILD_STATIC",
            "GLFW_INCLUDE_NONE"
        }

        links { "dl", "pthread" }

        defines { "_X11" }

project "Game"
    location "build/Game"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/Intermediate/" .. outputdir .. "/%{prj.name}")
    debugdir("build/bin/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/%{prj.name}/**.h",
        "src/%{prj.name}/**.cpp",
    }

    includedirs
    {
        "src/%{prj.name}/",
        "src/Blazar/"
    }

    includedirs(library_includes)


    links(library_links)
    links("Blazar")

    filter "configurations:Debug"
        defines ({"BLAZAR_DEBUG", "BLAZAR_CONSOLE_WINDOW"})
        symbols "On"
        runtime "Debug"
        kind "ConsoleApp"
        defines("TRACY_ENABLE")

    filter "configurations:Release"
        defines "BLAZAR_RELEASE"
        optimize "On"
        runtime "Release"
        defines("TRACY_ENABLE")

    filter "configurations:Distrobution"
        defines ({"BLAZAR_DIST", "BLAZAR_RELEASE"})
        optimize "Full"
        runtime "Release"

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "BLAZAR_PLATFORM_WINDOWS",
            "BLAZAR_BUILD_STATIC"
        }
        postbuildcommands {
            'xcopy "%{wks.location}..\\assets" "..\\bin\\%{outputdir}\\%{prj.name}\\Contents\\" /Q /E /Y /I'
        }

    filter "system:linux"
        defines
        {
            "BLAZAR_PLATFORM_LINUX",
            "BLAZAR_BUILD_STATIC"
        }

        links { "dl", "pthread", "GL", "X11", "GLAD"}

        defines { "_X11" }

        postbuildcommands {
            "cp -r %{wks.location}/../assets ../bin/%{outputdir}/%{prj.name}/Contents/"
        }
