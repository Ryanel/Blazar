
group("Vendor")
IncludeDir["entt"]     = "src/Vendor/entt/src/"
project "entt"
    kind "StaticLib"
    language "C++"

    targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{prj.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

    files("./entt/*.hpp")
    includedirs("./entt/*.hpp")

    filter "configurations:Debug"
      runtime "Debug"
      symbols "on"
    filter "configurations:Release"
      runtime "Release"
      optimize "on"
    filter "system:windows"
        systemversion "latest"

group("")