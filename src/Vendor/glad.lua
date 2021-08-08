outputdir="%{cfg.buildcfg}-%{cfg.system}"

IncludeDir["GLAD"] = "src/Vendor/GLAD/include/"

group("Vendor")
project "GLAD"
    kind "StaticLib"
    language "C"

    targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{prj.location}/Intermediate/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "glad/include/glad/glad.h",
        "glad/include/KHR/khrplatform.h",
        "glad/src/glad.c"
    }

    includedirs("glad/include")

    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"


group("")