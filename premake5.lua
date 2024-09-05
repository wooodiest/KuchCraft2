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

project "KuchCraft2"
    kind       "ConsoleApp"
    language   "C++"
    cppdialect "C++20"
    location   "KuchCraft2"
    targetdir ("%{wks.location}/bin/"     .. outputdir .. "/%{prj.name}")
    objdir    ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{wks.location}/KuchCraft2/src/**.h",
        "%{wks.location}/KuchCraft2/src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/KuchCraft2/src"
    }

    filter "system:windows"
        systemversion "latest"

    defines
    { 
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter   "configurations:Debug"
    defines  "KC_Debug"
    runtime  "Release"
    optimize "on"
    symbols  "On"

    filter   "configurations:Release"
    defines  "KC_Release"
    runtime  "Release"
    optimize "on"
    symbols  "Off"
