V_MAJOR = 1
V_MINOR = 0
ENGINE_NAME = "banana"
PROJECT_NAME = "bananaEditor"
import sys,platform

PLATFORM = sys.platform
for x in platform.uname():
    if "microsoft" in x.lower():
        PLATFORM = "win32"

def IsWindows():
    return PLATFORM == "win32"
def IsLinux():
    return PLATFORM == "linux"
def IsMac():
    return PLATFORM == "darwin"
