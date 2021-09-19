# cli build
# cli gen
# cli version
# cli run
# cli gen build version

import sys
import subprocess

TOOLS_DIR = "tools"

def runCommand(cmd):
    subprocess.call(["python", "{}/{}.py".format(TOOLS_DIR,cmd)])

for i in range(1,len(sys.argv)):
    cmd = sys.argv[i] 
    
    print("\n----------------------")
    print("Executing : ",cmd)

    runCommand(cmd)