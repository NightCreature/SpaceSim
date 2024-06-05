import sys
import os
import subprocess
#..\..\..\SDK\DirectXCompiler\bin\x64\dxc.exe -E ps_main -T ps_6_6 -Zs -Fo .\Compiled\coreps.bin -Fc .\Compiled\Coreps.asm -Fd .\Compiled\coreps.pdb -Qstrip_reflect -Frs .\Compiled\Coreps.rs -HV 2021 .\core_pixel_shader.ps

def main():
    filename, fileextenstion = os.path.splitext(sys.argv[1])
    filename = filename.replace('.', '')
    filename = filename.replace('/', '')
    filename = filename.replace('\\', '')

    prefix = ""
    if (fileextenstion == ".ps"):
        prefix = "ps"
    elif (fileextenstion == ".vs"):
        prefix = "vs"
    else:
        #check out the 3rd commandline argument for the prefix of our entry and profile
        for arg in sys.argv:
            print(arg)
        # if (sys.argv[2] != "") :
        #     print(sys.argv[2])
        #     prefix = sys.argv[2]
        # else:
        #     print("Should provide entry point and profile")

    # entrypoint = prefix + "_main"
    # profile = prefix + "_6_6"
    # ouputfilename = "./Compiled/" + filename + ".bin"
    # outputPdb = "./Compiled/" + filename + ".pdb"
    # outputrs = "./Compiled/" + filename + ".rs"
    # outputasm = "./Compiled/" + filename + ".asm"

    # command = os.getcwd() + "/../../../SDK/DirectXCompiler/bin/x64/dxc.exe"
    # commandline = command + " -E " + entrypoint + " -T " + profile + " -Zs -Fo " + ouputfilename + " -Fc " + outputasm + " -Fd " + outputPdb + " -Frs " + outputrs +  " -HV 2021 " +  filename + fileextenstion
    # print(subprocess.check_output(commandline))

if __name__ == "__main__":
    main()