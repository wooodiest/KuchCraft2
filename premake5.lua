workspace "KuchCraft2"
    startproject "KuchCraft2"
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    architecture "x64"
    configurations 
    { 
        "Debug",
        "Release"
    }
    flags
    { 
        "MultiProcessorCompile" 
    }

group "Dependencies"
	include "KuchCraft2/vendor/glfw"
    include "KuchCraft2/vendor/glad"
    include "KuchCraft2/vendor/imgui"
group ""

project "KuchCraft2"
    kind       "WindowedApp"
    language   "C++"
    cppdialect "C++20"
    location   "KuchCraft2"
    targetdir ("%{wks.location}/bin/"     .. outputdir .. "/%{prj.name}")
    objdir    ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "kcpch.h"
    pchsource "%{wks.location}/KuchCraft2/src/kcpch.cpp"

    files
    {
        "%{wks.location}/KuchCraft2/src/**.h",
        "%{wks.location}/KuchCraft2/src/**.cpp",
        "%{wks.location}/KuchCraft2/vendor/glm/glm/**.hpp",
        "%{wks.location}/KuchCraft2/vendor/glm/glm/**.inl",
        "%{wks.location}/KuchCraft2/vendor/stb_image/**.h",
		"%{wks.location}/KuchCraft2/vendor/stb_image/**.cpp",
    }

    includedirs
    {
        "%{wks.location}/KuchCraft2/src",
        "%{wks.location}/KuchCraft2/vendor/spdlog/include",
        "%{wks.location}/KuchCraft2/vendor/glm",
        "%{wks.location}/KuchCraft2/vendor/glfw/include",
        "%{wks.location}/KuchCraft2/vendor/glad/include",
        "%{wks.location}/KuchCraft2/vendor/imgui",
        "%{wks.location}/KuchCraft2/vendor/nlohmann_json",
        "%{wks.location}/KuchCraft2/vendor/entt",
        "%{wks.location}/KuchCraft2/vendor/stb_image"
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib",
        "ImGui"
    }

    filter "system:windows"
        systemversion "latest"
        links { "kernel32", "user32" }
        entrypoint "WinMainCRTStartup"

    filter "action:vs*"
        linkoptions { "/NODEFAULTLIB:LIBCMT" }
    filter {}

    defines
    { 
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    filter   "configurations:Debug"
    defines  "KC_DEBUG"
    runtime  "Release"
    optimize "on"
    symbols  "On"

    filter   "configurations:Release"
    defines  "KC_RELEASE"
    runtime  "Release"
    optimize "on"
    symbols  "Off"
