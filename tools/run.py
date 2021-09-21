import subprocess,globals,os

CONFIG = "Debug"
exePath = "./bin/{}/{}".format(CONFIG,globals.PROJECT_NAME)
if globals.IsWindows():
    subprocess.call(["cmd.exe","/c","cd {} && {}.exe".format(exePath,globals.PROJECT_NAME)])
else:
    subprocess.call([os.getcwd()+"{}/{}".format(exePath,globals.PROJECT_NAME)[1:]],cwd=exePath)