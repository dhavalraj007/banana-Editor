workspace "banana"
    startproject "bananaEditor"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

project "bananaEditor"
    location "bananaEditor"
    kind "ConsoleApp"
    
    language "C++"
    cppdialect "C++17"
    flags
    {
        "FatalWarnings"
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }