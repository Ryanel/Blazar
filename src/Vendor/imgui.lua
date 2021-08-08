group("Vendor")

-- ImGUI
IncludeDir["IMGUI"]    = "src/Vendor/imgui/"
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
        "./imgui/backends/imgui_impl_glfw.h",
        "./imgui/misc/cpp/*.h",
        "./imgui/misc/cpp/*.cpp",
        "./imgui/misc/debuggers/*"
    }

    includedirs {
        "./imgui/",
        "./imgui/misc/cpp/",
        "./GLAD/include/",
        "./glfw/include/"
    }

    links { "GLAD", "GLFW" }

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

group("")