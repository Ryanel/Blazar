outputdir="%{cfg.buildcfg}-%{cfg.system}"

group("Vendor")
project "STB"
    kind "StaticLib"
    language "C"

    targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{prj.location}/Intermediate/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "stb_image.cpp",
        "stb_image.h"
    }

    includedirs {
        "./"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        optimize "On"
        runtime "Release"


group("")