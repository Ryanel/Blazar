outputdir="%{cfg.buildcfg}-%{cfg.system}"

-- Header only libraries here
IncludeDir["STB"]      = "src/Vendor/stb/"
IncludeDir["spdlog"]   = "src/Vendor/spdlog/include/"

-- Include other projects that require compilation
include "entt.lua"
include "glfw.lua"
include "tracy.lua"
include "imgui.lua"
include "glm.lua"
include "glad.lua"
include "stb.lua"

library_includes={
    "%{IncludeDir.GLAD}",
    "%{IncludeDir.IMGUI}",
    "%{IncludeDir.STB}",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.GLM}",
    "%{IncludeDir.Tracy}",
    "%{IncludeDir.entt}"
}

library_links={
    "GLAD",
    "IMGUI",
    "STB",
    "GLFW",
    "Tracy"
}
