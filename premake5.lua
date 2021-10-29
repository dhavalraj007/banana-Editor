workspace "banana"
    startproject "bananaEditor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

tdir = "bin/%{cfg.buildcfg}/%{prj.name}"
odir = "bin-obj/%{cfg.buildcfg}/%{prj.name}"
-- External Dependencies
externals = {}
externals["sdl2"] = "external/sdl2"
externals["spdlog"] = "external/spdlog"
externals["glad"] = "external/glad"

-- process glad before anything else
include "external/glad"

project "banana"
    location "banana"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    flags
    {
        "FatalWarnings"
    }

    targetdir(tdir)
    objdir(odir)
    
    sysincludedirs
    {
        "%{prj.name}/include",
        "%{externals.sdl2}/include",
        "%{externals.spdlog}/include",
        "%{externals.glad}/include"
    }

    files
    {
        "%{prj.name}/include/**.h",     --only this will be exposed to outer projects
        "%{prj.name}/include/**.cpp",   
        "%{prj.name}/include/**.hpp",   
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/**.natvis"
    }
    
    defines
    {
        "GLFW_INCLUDE_NONE" -- ensure glad doesn't include glfw
    }

    filter { "system:windows","configurations:*"}
        systemversion "latest"

        defines
        {
            "BANANA_PLATFORM_WINDOWS"
        }
    
    filter { "system:macosx","configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }
        defines
        {
            "BANANA_PLATFORM_MAC"
        }
    
    filter { "system:linux","configurations:*"}

        defines
        {
            "BANANA_PLATFORM_LINUX"
        }
    
    filter {"configurations:Debug"}
        
        defines
        {
            "BANANA_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"
    
    filter {"configurations:Release"}
        
        defines
        {
            "BANANA_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"

project "bananaEditor"
    location "bananaEditor"
    kind "ConsoleApp"
    
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    links "banana"
    flags
    {
        "FatalWarnings"
    }

    targetdir(tdir)
    objdir(odir)
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    sysincludedirs
    {
        "banana/include",
        "%{externals.spdlog}/include"
    }

    filter { "system:windows","configurations:*"}
        systemversion "latest"

        defines
        {
            "BANANA_PLATFORM_WINDOWS"
        }

        libdirs
        {
            "%{externals.sdl2}/lib"
        }

        links
        {
            "SDL2",
            "glad"
        }
    
    filter { "system:macosx","configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }
        defines
        {
            "BANANA_PLATFORM_MAC"
        }
    
    filter { "system:linux","configurations:*"}

        defines
        {
            "BANANA_PLATFORM_LINUX"
        }
        links
        {
            "SDL2",
            "glad"
        }
    
    filter {"configurations:Debug"}
        
        defines
        {
            "BANANA_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"
    
    filter {"configurations:Release"}
        
        defines
        {
            "BANANA_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"

project "Pong"
    location "Pong"
    kind "ConsoleApp"
    
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    links "banana"
    flags
    {
        "FatalWarnings"
    }

    targetdir(tdir)
    objdir(odir)
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    sysincludedirs
    {
        "banana/include",
        "%{externals.spdlog}/include"
    }

    filter { "system:windows","configurations:*"}
        systemversion "latest"

        defines
        {
            "BANANA_PLATFORM_WINDOWS"
        }

        libdirs
        {
            "%{externals.sdl2}/lib"
        }

        links
        {
            "SDL2",
            "glad"
        }
    
    filter { "system:macosx","configurations:*"}
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "10.15",
            ["UseModernBuildSystem"] = "NO"
        }
        defines
        {
            "BANANA_PLATFORM_MAC"
        }
    
    filter { "system:linux","configurations:*"}

        defines
        {
            "BANANA_PLATFORM_LINUX"
        }
        links
        {
            "SDL2",
            "glad"
        }
    
    filter {"configurations:Debug"}
        
        defines
        {
            "BANANA_CONFIG_DEBUG"
        }
        runtime "Debug"
        symbols "on"
    
    filter {"configurations:Release"}
        
        defines
        {
            "BANANA_CONFIG_RELEASE"
        }
        runtime "Release"
        symbols "off"
        optimize "on"