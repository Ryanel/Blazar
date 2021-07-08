outputdir="%{cfg.buildcfg}-%{cfg.system}"

group("Vendor")

--- GLFW
project "GLFW"
kind "StaticLib"
language "C"

targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir("%{prj.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

files
{
    "glfw/include/GLFW/glfw3.h",
    "glfw/include/GLFW/glfw3native.h",
    "glfw/src/glfw_config.h",
    "glfw/src/context.c",
    "glfw/src/init.c",
    "glfw/src/input.c",
    "glfw/src/monitor.c",
    "glfw/src/vulkan.c",
    "glfw/src/window.c"
}

    filter "system:linux"
        pic "On"

        systemversion "latest"

        files
        {
            "glfw/src/x11_init.c",
            "glfw/src/x11_monitor.c",
            "glfw/src/x11_window.c",
            "glfw/src/xkb_unicode.c",
            "glfw/src/posix_time.c",
            "glfw/src/posix_thread.c",
            "glfw/src/glx_context.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c",
            "glfw/src/linux_joystick.c"
        }

        defines
        {
            "_GLFW_X11"
        }

        links {
            "Xrandr",
            "Xi",
            "GLU",
            "GL",
            "X11",
            "dl",
            "pthread",
        }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "glfw/src/win32_init.c",
            "glfw/src/win32_joystick.c",
            "glfw/src/win32_monitor.c",
            "glfw/src/win32_time.c",
            "glfw/src/win32_thread.c",
            "glfw/src/win32_window.c",
            "glfw/src/wgl_context.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c"
        }

        defines 
        { 
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"


-- ImGUI
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
        "./GLAD/include/",
        "./glfw/include/"
    }

    links {
        "GLAD",
        "GLFW"
    }

    filter "system:windows"
        systemversion "latest"

    filter "system:linux"
        defines("IMGUI_IMPL_OPENGL_LOADER_GLAD")

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"


-- GLM
project "GLM"
    kind "StaticLib"
    language "C++"

    targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{prj.location}/Intermediate/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "./glm/glm/*.h"
    }

    includedirs {
        "./glm/"
    }

    filter "system:windows"
        systemversion "latest"

        
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"


-- GLM
project "Tracy"
    kind "StaticLib"
    language "C++"

    targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{prj.location}/Intermediate/" .. outputdir .. "/%{prj.name}")
    
    defines("TRACY_ENABLE")

    files
    {
        "./tracy/TracyClient.cpp"
    }

    includedirs {
        "./tracy/*.hpp"
    }

    filter "configurations:Debug"
      runtime "Debug"
      symbols "on"

    filter "configurations:Release"
      runtime "Release"
      optimize "on"

    filter "system:windows"
        systemversion "latest"

group("")