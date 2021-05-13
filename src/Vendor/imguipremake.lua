outputdir="%{cfg.buildcfg}-%{cfg.system}"

group("Vendor")
project "IMGUI"
    kind "StaticLib"
    language "C++"

    targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{prj.location}/Intermediate/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "./imgui/*.h",
        "./imgui/*.cpp",
        "./imgui/backends/imgui_impl_opengl3.cpp",
        "./imgui/backends/imgui_impl_opengl3.h",
        "./imgui/backends/imgui_impl_glfw.cpp",
        "./imgui/backends/imgui_impl_glfw.h"
    }

    includedirs {
        "./imgui/",
        "./GLAD/include/"
    }

    links {
        "GLAD"
    }


    filter "system:windows"
        systemversion "latest"
        staticruntime "On"


group("")