import globals,subprocess

CONFIG = "debug"

if globals.IsWindows():
    VS_BUILD_PATH = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\MSBuild\\Current\\Bin\\MSBuild.exe"
    subprocess.call(["cmd.exe","/c",VS_BUILD_PATH,"{}.sln".format(globals.ENGINE_NAME),"/property:Configuration={}".format(CONFIG)])