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
        "%{prj.name}/include/banana",
        "%{externals.sdl2}/include"
    }

    files
    {
        "%{prj.name}/include/**.h",     --only this will be exposed to outer projects
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
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
        "banana/include"
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
            "SDL2"
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
