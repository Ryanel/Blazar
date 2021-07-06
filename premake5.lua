-- premake5.lua

-- Conan
editorintegration "On"

workspace "Blazar"
    architecture "x64"
    configurations { "Debug", "Release", "Distribution" }
    location "build"

outputdir="%{cfg.buildcfg}-%{cfg.system}"

IncludeDir= {}
IncludeDir["GLAD"]     = "src/Vendor/GLAD/include/"
IncludeDir["IMGUI"]    = "src/Vendor/imgui/"
IncludeDir["STB"]      = "src/Vendor/stb/"
IncludeDir["spdlog"]   = "src/Vendor/spdlog/include/"
IncludeDir["GLFW"]     = "src/Vendor/glfw/include/"
IncludeDir["GLM"]      = "src/Vendor/glm/"

include "src/Vendor/GLAD"
include "src/Vendor/vendor.lua"
include "src/Vendor/stb/premake5.lua"

project "Blazar"
    location "build/Blazar/"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir ("build/bin/" .. outputdir .. "/%{prj.name}")
    objdir("build/Intermediate/" .. outputdir .. "/%{prj.name}")

    pchheader "bzpch.h"
    pchsource "src/Blazar/bzpch.cpp"

    links {
        "GLAD",
        "IMGUI",
        "STB",
        "GLFW"
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
        "%{IncludeDir.STB}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLM}"
    }

    defines ("BLAZAR_IMGUI_ENABLED")

    filter "configurations:Debug"
        symbols "On"
        runtime "Debug"
        defines ({"BLAZAR_ENABLE_ASSERTS", "BLAZAR_DEBUG"})

    filter "configurations:Release"
        defines ({"BLAZAR_ENABLE_ASSERTS", "BLAZAR_RELEASE"})
        optimize "On"
        runtime "Release"

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
            "GLFW_INCLUDE_NONE"
        }

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
        "src/Blazar/",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.STB}"
    }

    links
    {
        "Blazar",
        "IMGUI",
        "GLFW",
        "GLAD",
        "STB"
    }

    filter "configurations:Debug"
        defines "BLAZAR_DEBUG"
        symbols "On"
        runtime "Debug"
        defines {
            "BLAZAR_ENABLE_ASSERTS",
            "BLAZAR_IMGUI_ENABLED",
            "BLAZAR_CONSOLE_WINDOW",
            
        }
        kind "ConsoleApp"

    filter "configurations:Release"
        defines "BLAZAR_RELEASE"
        defines {
            "BLAZAR_ENABLE_ASSERTS",
            "BLAZAR_IMGUI_ENABLED"
        }
        optimize "On"
        runtime "Release"

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