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
IncludeDir["IMGUI"]    = "src/Vendor/IMGUI/"
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

    linkoptions { conan_exelinkflags }

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
        "%{IncludeDir.GLM}",
        conan_includedirs
    }

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

project "Game"
    location "build/Game"
    kind "ConsoleApp"
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
        conan_includedirs
    }

    links
    {
        "Blazar",
        "IMGUI",
        "GLFW"
    }

    postbuildcommands {
        "xcopy %{wks.location}..\\assets ..\\bin\\%{outputdir}\\%{prj.name}\\Contents\\ /Q /E /Y /I"
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