group("Vendor")

-- GLM
IncludeDir["GLM"]      = "src/Vendor/glm/"
project "GLM"
    kind "StaticLib"
    language "C++"

    targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{prj.location}/Intermediate/" .. outputdir .. "/%{prj.name}")
    
    files("./glm/glm/*.h")
    includedirs("./glm/")
        
    filter "system:windows"
        systemversion "latest"
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    filter "configurations:Release"
        runtime "Release"
        optimize "on"


group("")