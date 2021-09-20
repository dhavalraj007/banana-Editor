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

project "bananaEditor"
    location "bananaEditor"
    kind "ConsoleApp"
    
    language "C++"
    cppdialect "C++17"
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