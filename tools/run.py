import subprocess,globals,os,sys

CONFIG = "Debug"
exePath = "./bin/{}/{}".format(CONFIG,globals.PROJECT_NAME)
ret = 0

if globals.IsWindows():
    ret = subprocess.call(["cmd.exe","/c","cd {} && {}.exe".format(exePath,globals.PROJECT_NAME)])
else:
    ret = subprocess.call([os.getcwd()+"{}/{}".format(exePath,globals.PROJECT_NAME)[1:]],cwd=exePath)

sys.exit(ret)