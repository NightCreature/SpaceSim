import sys
import os
import subprocess
import shutil
#..\..\..\SDK\DirectXCompiler\bin\x64\dxc.exe -E ps_main -T ps_6_6 -Zs -Fo .\Compiled\coreps.bin -Fc .\Compiled\Coreps.asm -Fd .\Compiled\coreps.pdb -Qstrip_reflect -Frs .\Compiled\Coreps.rs -HV 2021 .\core_pixel_shader.ps

def compileShader(fileName, prefix, fileExtension, isDebug):
    entrypoint = prefix + "_main"
    profile = prefix + "_6_6"
    ouputfilename = "./Compiled/" + prefix + "_" + fileName + ".cso"
    outputPdb = "./Compiled/" #+ prefix + "_" + fileName + ".pdb"
    outputrs = "./Compiled/" + prefix + "_"  + fileName + ".rs"
    outputasm = "./Compiled/" + prefix + "_" + fileName + ".asm"

    generateFullPdbCommandlineArg = ""
    if (isDebug):
        generateFullPdbCommandlineArg = " -Zi"
    else:
        generateFullPdbCommandlineArg = " -Zs"

    command = os.getcwd() + "/../../../SDK/DirectXCompiler/bin/x64/dxc.exe"
    commandline = command + " -E " + entrypoint + " -T " + profile + generateFullPdbCommandlineArg + " -Zpr -Fo " + ouputfilename + " -Fc " + outputasm + " -Fd " + outputPdb + " -Frs " + outputrs +  " -HV 2021 " +  fileName + fileExtension
    print(commandline)
    print(subprocess.check_output(commandline))

def main():
    isDebug = 0
    for arg in sys.argv:
        if (arg == "debug"):
            isDebug = 1
    currentDir = os.getcwd();
    outputDirectory = currentDir + "/Compiled/"
    if (os.path.exists(outputDirectory)) :
        #delete contents.
        for filename in os.listdir(outputDirectory):
            filePath = os.path.join(outputDirectory, filename)
            try:
                if os.path.isfile(filePath) or os.path.islink(filePath):
                    os.unlink(filePath)
                elif os.path.isdir(filePath):
                    shutil.rmtree(filePath)
            except Exception as e:
                print('Failed to delete %s. Reason: %s' % (filePath, e))
    else:
        os.mkdir(outputDirectory)

    for file in os.listdir(currentDir):
        #if extention is vs,ps or fx these are shaders and we need to compile them
        fileName, fileExtention = os.path.splitext(file)
        shaderType = 0
        shaderPrefix = ""
        if (fileExtention == ".ps"):
            #pixel shader
            shaderPrefix = "ps"
            shaderType = 1
        if (fileExtention == ".vs"):
            #vertex shader
            shaderType = 2
            shaderPrefix = "vs"
        if (fileExtention == ".fx"):
            #combined shader
            shaderType = 3

        #dealing with valid shaders
        if (shaderType > 0 and shaderType < 4):
            print("Compiling shader: ")
            print(file)
            print("\n")
            #fx file run command twice
            if (shaderType != 3):
                compileShader(fileName, shaderPrefix, fileExtention, isDebug)
            else:
                compileShader(fileName, "ps", fileExtention, isDebug)
                compileShader(fileName, "vs", fileExtention, isDebug)

if __name__ == "__main__":
    main()