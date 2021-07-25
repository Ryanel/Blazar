-- premake5.lua

-- Conan
editorintegration "On"
editAndContinue "Off"
workspace "Blazar"
    architecture "x64"
    configurations { "Debug", "Release", "Distribution" }
    location "build"
    flags { 'MultiProcessorCompile' }
    defines("_ITERATOR_DEBUG_LEVEL=0")

outputdir="%{cfg.buildcfg}-%{cfg.system}"

IncludeDir= {}
IncludeDir["GLAD"]     = "src/Vendor/GLAD/include/"
IncludeDir["IMGUI"]    = "src/Vendor/imgui/"
IncludeDir["STB"]      = "src/Vendor/stb/"
IncludeDir["spdlog"]   = "src/Vendor/spdlog/include/"
IncludeDir["GLFW"]     = "src/Vendor/glfw/include/"
IncludeDir["GLM"]      = "src/Vendor/glm/"
IncludeDir["Tracy"]      = "src/Vendor/tracy/"
IncludeDir["entt"]      = "src/Vendor/entt/src/"

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
        "GLFW",
        "Tracy"
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
        "%{IncludeDir.Tracy}",
        "%{IncludeDir.entt}"
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

        disablewarnings({"26812", "26495", "26451", "26498", "6201"})

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
        "%{IncludeDir.STB}",
        "%{IncludeDir.Tracy}",
        "%{IncludeDir.entt}"
    }

    links
    {
        "Blazar",
        "IMGUI",
        "GLFW",
        "GLAD",
        "STB",
        "Tracy"
    }

    

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