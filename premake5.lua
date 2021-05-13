-- premake5.lua

-- Conan
editorintegration "On"
include("build/conanbuildinfo.premake.lua")

workspace "Blazar"
    conan_basic_setup()
    architecture "x64"
    configurations { "Debug", "Release", "Distribution" }
    location "build"

outputdir="%{cfg.buildcfg}-%{cfg.system}"

IncludeDir= {}
IncludeDir["GLAD"] = "src/Vendor/GLAD/include/"
IncludeDir["IMGUI"] = "src/Vendor/IMGUI/"

include "src/Vendor/GLAD"
include "src/Vendor/imguipremake.lua"

project "Blazar"
    location "build/Blazar/"
    kind "StaticLib"
    language "C++"

    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/Intermediate/" .. outputdir .. "/%{prj.name}")

    pchheader "bzpch.h"
    pchsource "src/Blazar/bzpch.cpp"

    linkoptions { conan_exelinkflags }

    links {
        "GLAD",
        "IMGUI"
    }

    files
    {
        "src/%{prj.name}/**.h",
        "src/%{prj.name}/**.cpp",
    }

    includedirs {
        "src/%{prj.name}/",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.IMGUI}",
        conan_includedirs
    }

    filter "configurations:Debug"
        defines "BLAZAR_DEBUG"
        symbols "On"
        runtime "Debug"
        defines {
            "BLAZAR_ENABLE_ASSERTS"
        }

    filter "configurations:Release"
        defines "BLAZAR_RELEASE"
        optimize "On"
        runtime "Release"

    filter "configurations:Distrobution"
        defines ({"BLAZAR_DIST", "BLAZAR_RELEASE"})
        optimize "Full"
        runtime "Release"

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines
        {
            "BLAZAR_PLATFORM_WINDOWS",
            "BLAZAR_BUILD_STATIC",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Game")
        }

project "Game"
     location "build/Game"
     kind "ConsoleApp"
     language "C++"

     targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
     objdir("build/Intermediate/" .. outputdir .. "/%{prj.name}")

     files
     {
         "src/%{prj.name}/**.h",
         "src/%{prj.name}/**.cpp",
     }
 
     includedirs
     {
         "src/%{prj.name}/Includes/",
         "src/Blazar/",
         "%{IncludeDir.GLAD}",
         "%{IncludeDir.IMGUI}",
         conan_includedirs
     }

     links
     {
        "Blazar",
        "IMGUI"
     }

     filter "configurations:Debug"
        defines "BLAZAR_DEBUG"
        symbols "On"
        runtime "Debug"
        defines {
            "BLAZAR_ENABLE_ASSERTS"
        }

    filter "configurations:Release"
        defines "BLAZAR_RELEASE"
        optimize "On"
        runtime "Release"

    filter "configurations:Distrobution"
        defines ({"BLAZAR_DIST", "BLAZAR_RELEASE"})
        optimize "Full"
        runtime "Release"

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "BLAZAR_PLATFORM_WINDOWS",
            "BLAZAR_BUILD_STATIC"
        }

project "Build Configuration"
    kind "None"
    files
    {
        "premake5.lua",
        "conanfile.txt"
    }