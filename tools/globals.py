V_MAJOR = 1
V_MINOR = 0

import sys,platform

PLATFORM = sys.platform
for x in platform.uname():
    if "microsoft" in x.lower():
        PLATFORM = "windows"

def IsWindows():
    return PLATFORM == "windows"
def IsLinux():
    return PLATFORM == "linux"
def IsWindows():
    return PLATFORM == "darwin"
