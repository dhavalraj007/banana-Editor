import sys,os
import subprocess

TOOLS_DIR = "tools"

def runCommand(cmd):
    ret = 0
    scriptPath = "{}/{}.py".format(TOOLS_DIR,cmd)
    if os.path.exists(scriptPath):
        print("Executing : ",cmd)
        ret = subprocess.call(["python3", scriptPath])
    else:
        print("Invalid Command : ",cmd)
        ret = -1
    
    return ret



for i in range(1,len(sys.argv)):
    cmd = sys.argv[i] 
    print("\n----------------------")
    if runCommand(cmd) != 0:
        break