import subprocess,globals

CONFIG = "Debug"
exePath = ".\\bin\\{}\\{}".format(CONFIG,globals.PROJECT_NAME)
if globals.IsWindows():
    subprocess.call(["cmd.exe","/c","cd {} && {}.exe".format(exePath,globals.PROJECT_NAME)])
else:
    subprocess.call(["cd {} && {}".format(exePath,globals.PROJECT_NAME)])