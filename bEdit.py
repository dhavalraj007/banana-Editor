# cli build
# cli gen
# cli version
# cli run
# cli gen build version

import sys,os
import subprocess

TOOLS_DIR = "tools"

def runCommand(cmd):
    scriptPath = "{}/{}.py".format(TOOLS_DIR,cmd)
    if os.path.exists(scriptPath):
        print("Executing : ",cmd)
        subprocess.call(["python3", scriptPath])
    else:
        print("Invalid Command : ",cmd)



for i in range(1,len(sys.argv)):
    cmd = sys.argv[i] 
    
    print("\n----------------------")
    

    runCommand(cmd)