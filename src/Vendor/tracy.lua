group("Vendor")
IncludeDir["Tracy"]    = "src/Vendor/tracy/"

project "Tracy"
    kind "StaticLib"
    language "C++"

    targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{prj.location}/Intermediate/" .. outputdir .. "/%{prj.name}")
    
    defines("TRACY_ENABLE")
    files("./tracy/TracyClient.cpp")
    includedirs("./tracy/*.hpp")

    filter "configurations:Debug"
      runtime "Debug"
      symbols "on"
    filter "configurations:Release"
      runtime "Release"
      optimize "on"
    filter "system:windows"
        systemversion "latest"
group("")