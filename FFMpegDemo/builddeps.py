#! /usr/bin/env python3
# -*- coding: utf-8 -*- 
#
# builddeps.py
#
# Created by Ruibin.Chow on 2022/01/26.
# Copyright (c) 2022年 Ruibin.Chow All rights reserved.
# 

"""

"""


import os, re, json, sys, platform, fnmatch, stat
import subprocess, shutil, json
import datetime
import tarfile, gzip, zipfile, bz2
import urllib.request
from pathlib import Path
import multiprocessing


homeDir = ""
thirdPartyDir = ""
sourceDir = ""
outputDir = ""
thirdPartyDirName = "third_party"
sourceDirName = "depsSource"
outputDirName = "deps"
depsName = "deps.json"
sourceLockName = sourceDirName + ".lock"
buildDir = "buildGen" # cmake构建目录
cmakeOther = ""
libSufixs = [".a", ".lib", ".so", ".dylib", ".dll"]
depsSourceFlag = False

CPU_COUNT = multiprocessing.cpu_count()
DEPS_ARCH = "DEPS_ARCH"


logList = []

def logRecord():
    with open(os.path.join(homeDir, "builddeps.log"), "w") as fileHandle:
        for logStr in logList:
            fileHandle.write(str(logStr))

def log(string="", newline=True):
    if newline:
        logList.append(str(string) + "\n")
        print(string, end="\n")
    else:
        logList.append(str(string))
        print(string, end="")
    pass

def operator(cmdString, newline=True):
    log(cmdString)
    output = os.popen(cmdString)
    for line in output.readlines():
        log(line, newline)

def operatorCMD(parameterList, newline=True):
    cmdString = " ".join(parameterList)
    operator(cmdString, newline)
    pass

def getAllFileInDirectory(DIR, beyoundDir=''):
    """返回指定目录下所有文件的集合，beyoundDir的目录不包含"""
    array = []
    print(DIR+beyoundDir)
    for root, dirs, files in os.walk(DIR):
        if len(beyoundDir) != 0 and os.path.exists(DIR+beyoundDir):
            if beyoundDir not in dirs:
                continue
        for name in files:
            path = os.path.join(root,name)
            array.append(path)
    return array

def json_minify(string, strip_space=True):
    """
    A port of the `JSON-minify` utility to the Python language.
    Based on JSON.minify.js: https://github.com/getify/JSON.minify
    """
    tokenizer = re.compile('"|(/\*)|(\*/)|(//)|\n|\r')
    end_slashes_re = re.compile(r'(\\)*$')

    in_string = False
    in_multi = False
    in_single = False

    new_str = []
    index = 0
    for match in re.finditer(tokenizer, string):
        if not (in_multi or in_single):
            tmp = string[index:match.start()]
            if not in_string and strip_space:
                # replace white space as defined in standard
                tmp = re.sub('[ \t\n\r]+', '', tmp)
            new_str.append(tmp)
        elif not strip_space:
            # Replace comments with white space so that the JSON parser reports
            # the correct column numbers on parsing errors.
            new_str.append(' ' * (match.start() - index))

        index = match.end()
        val = match.group()

        if val == '"' and not (in_multi or in_single):
            escaped = end_slashes_re.search(string, 0, match.start())

            # start of string or unescaped quote character to end string
            if not in_string or (escaped is None or len(escaped.group()) % 2 == 0):  # noqa
                in_string = not in_string
            index -= 1  # include " character in next catch
        elif not (in_string or in_multi or in_single):
            if val == '/*':
                in_multi = True
            elif val == '//':
                in_single = True
        elif val == '*/' and in_multi and not (in_string or in_single):
            in_multi = False
            if not strip_space:
                new_str.append(' ' * len(val))
        elif val in '\r\n' and not (in_multi or in_string) and in_single:
            in_single = False
        elif not ((in_multi or in_single) or (val in ' \r\n\t' and strip_space)):  # noqa
            new_str.append(val)

        if not strip_space:
            if val in '\r\n':
                new_str.append(val)
            elif in_multi or in_single:
                new_str.append(' ' * len(val))

    new_str.append(string[index:])
    return ''.join(new_str)

def swapDepsArgs(args):
    if args == None:
        return ""

    if "//" in args:
        args = args.replace("//", homeDir+os.path.sep)

    if DEPS_ARCH in args:
        args = args.replace(DEPS_ARCH, platform.machine())
    
    return args


def configBuild(fileName, configArgs, targetDir=None, genBuilding=True, install=True):
    os.chdir(fileName)

    if targetDir != None:
        os.chdir(targetDir)

    if genBuilding:
        if os.path.exists(buildDir):
            shutil.rmtree(buildDir)
        os.makedirs(buildDir)
        os.chdir(buildDir)

    log("-"*80)
    log("当前编译路径：" + os.getcwd())
    log("configBuild: " + fileName)

    configArgs = swapDepsArgs(configArgs)

    os.chmod("../configure", stat.S_IEXEC|stat.S_IRWXU|stat.S_IRWXG|stat.S_IRWXO)
    cmdStr = "../configure " + "--prefix=" + outputDir + " " + configArgs
    makeStr = "make -j" + str(CPU_COUNT)
    makeInstall = "make install"

    if platform.machine() == "arm64" and platform.system() == "Darwin":
        cmdStr = "arch -arm64e " + cmdStr
        makeStr = "arch -arm64e " + makeStr
        makeInstall = "arch -arm64e " + makeInstall

    operator(cmdStr, False)
    operator(makeStr, False)
    operator(makeInstall, False)
    pass


def cmakeBuild(fileName, cmakeArgs, targetDir=None, genBuilding=True, preCmdList=[], install=True):
    os.chdir(fileName)

    if targetDir != None:
        os.chdir(targetDir)

    if genBuilding:
        if os.path.exists(buildDir):
            shutil.rmtree(buildDir)
        os.makedirs(buildDir)
        os.chdir(buildDir)

    log("-"*80)
    log("当前编译路径：" + os.getcwd())
    if len(preCmdList) > 0:
        operatorCMD(preCmdList, False)

    cmakeArgs = swapDepsArgs(cmakeArgs)

    otherCmakeArgs = ""
    outputBinDir = os.path.join(outputDir, "bin")
    if os.path.exists(outputBinDir):
        exeFiles = os.listdir(outputBinDir)
        for exeFile in exeFiles:
            if fnmatch.fnmatch(exeFile, "ninja*"):
                otherCmakeArgs = otherCmakeArgs + "-GNinja -DCMAKE_MAKE_PROGRAM="
                otherCmakeArgs = otherCmakeArgs + os.path.join(outputBinDir, exeFile) + " "
                break

    osName = platform.system()
    if(osName == 'Windows'):
        log("Warning Windows.")
    elif(osName == 'Linux'):
        log("Warning Linux.")
    elif(osName == 'Darwin'):
        if platform.machine() == "arm64":
            otherCmakeArgs = otherCmakeArgs + "-DCMAKE_OSX_ARCHITECTURES=arm64 "
            otherCmakeArgs = otherCmakeArgs + "-DCMAKE_HOST_SYSTEM_PROCESSOR=arm64 "
            # otherCmakeArgs = otherCmakeArgs + "-DCMAKE_SYSTEM_PROCESSOR=arm64 "
            # otherCmakeArgs = otherCmakeArgs + "-DCMAKE_HOST_SYSTEM_PROCESSOR=arm64 "

    cmdList = ["cmake",
                cmakeArgs,
                "-DCMAKE_BUILD_TYPE=RELEASE",
                otherCmakeArgs,
                "-DCMAKE_INSTALL_PREFIX="+outputDir, 
                "..",
                ]
    operatorCMD(cmdList, False)
    if install:
        operator("cmake --build . --target install", False)
    pass


def untar(fname, dirs):
    """
    解压tar.gz文件
    :param fname: 压缩文件名
    :param dirs: 解压后的存放路径
    :return: bool
    """
    try:
        t = tarfile.open(fname)
        t.extractall(path = dirs)
        return True
    except Exception as e:
        print(e)
        return False

def zipExtract(path_zip, path_aim):
    z = zipfile.ZipFile(path_zip, 'r')
    for p in z.namelist():
        z.extract(p, path_aim)
    z.close()
    depressName = os.path.split(z.namelist()[0])[0]
    desName = os.path.splitext(path_zip)[0]
    if depressName != desName:
        os.rename(depressName, desName)


def bz2Extract(path_bz2, path_aim):
    print("bz2Extract: " + path_bz2)
    archive = tarfile.open(path_bz2,'r:bz2')
    # archive.debug = 1    # Display the files beeing decompressed.
    for tarinfo in archive:
        archive.extract(tarinfo, path_aim) 
    archive.close()


def callbackfunc(blocknum, blocksize, totalsize):
    '''回调函数
    @blocknum: 已经下载的数据块
    @blocksize: 数据块的大小
    @totalsize: 远程文件的大小
    '''
    percent = 100.0 * blocknum * blocksize / totalsize
    if percent > 100:
        percent = 100
    print("文件下载:%.2f%%"% percent)

def downloadFile(url, dirPath):
    urllib.request.urlretrieve(url, dirPath, callbackfunc)
    pass


def isDesps(fileName):
    lockFile = os.path.join(sourceDir, sourceLockName)
    if not os.path.exists(lockFile):
        return False
    with open(lockFile, "r") as f:
        for line in f:
            line = line.replace(os.linesep, "")
            if line == fileName:
                return True
            else:
                continue
    return False

def updateDesps(fileName):
    lockFile = os.path.join(sourceDir, sourceLockName)
    with open(lockFile, "a") as f:
        f.writelines(fileName + os.linesep)


def getDictValues(depsDict):
    fileName = depsDict["fileName"]
    action = depsDict["action"]
    url = depsDict["url"]

    buildAction = "cmake"
    if "build" in depsDict:
        buildAction = depsDict["build"]

    args = None
    if "args" in depsDict:
        args = depsDict["args"]

    targetDir = None
    if "target_dir" in depsDict:
        targetDir = depsDict["target_dir"]

    return fileName, action, url, buildAction, args, targetDir


def downloadAndBuild(depsDict):
    fileName, action, url, buildAction, args, targetDir = getDictValues(depsDict)

    if action == "gz": action = "tar.gz"
    if action == "bz2": action = "tar.bz2" 

    if len(fileName) == 0:
        log("Download Was Error!")
        return

    os.chdir(sourceDir) # 进入到源代码存放的目录

    filePath = fileName + "." + action
    if not os.path.exists(filePath):
        log("Begin Download: " + fileName)
        downloadFile(url, filePath)
    if not os.path.exists(fileName):
        log("解压: " + filePath)
        fileType = os.path.splitext(filePath)[-1]
        if fileType == ".gz": untar(filePath, sourceDir)
        if fileType == ".zip": zipExtract(filePath, sourceDir)
        if fileType == ".bz2": bz2Extract(filePath, sourceDir)

    if buildAction == "config":
        configBuild(fileName, args, targetDir)
    else:
        cmakeBuild(fileName, args, targetDir, genBuilding=True, preCmdList=[], install=True)
    pass


def buildDeps(depsDict):
    fileName, action, url, buildAction, args, targetDir = getDictValues(depsDict)
    if len(fileName) == 0:
        log("Building Deps Was Error!")
        return
    log("-"*80)
    log("Start Building Deps: " + fileName)
    os.chdir(sourceDir) #进入到源代码存放的目录
    operator(url)

    if buildAction == "config":
        configBuild(fileName, args, targetDir)
    else:
        cmakeBuild(fileName, args, targetDir, genBuilding=True, preCmdList=[], install=True)
    pass


def buildThirdParty():
    os.chdir(homeDir)
    os.chdir(thirdPartyDir) #进入到第三方存放的目录
    # runDir = Path(thirdPartyDir)
    # folders = runDir.iterdir()
    folders = os.listdir(thirdPartyDir)
    for folder in folders:
        if not os.path.exists(os.path.join(folder, "CMakeLists.txt")):
            continue
        log("-"*80)
        log("folder: " + str(folder))
        fileName = str(folder)
        cmakeArgs = "-DCMAKE_CXX_STANDARD=14"
        cmakeBuild(fileName, cmakeArgs, None, genBuilding=True, preCmdList=[], install=True)
        os.chdir(thirdPartyDir) #回到第三方代码存放的目录
    pass


def buildFromDepsFile():
    os.chdir(homeDir)
    if not os.path.exists(depsName):
        log("Error: " + str(depsName) + " was not exist.")
        return

    depsJson = None
    with open(depsName, 'r', encoding='utf-8') as fw:
        # json.dump(json_str, fw, indent=4, ensure_ascii=False)
        jsonString = json_minify(fw.read())
        depsJson = json.loads(jsonString)
    if depsJson is None: 
        return
    for depsDict in depsJson:
        # log("depDict: " + str(depsDict))
        action = depsDict["action"]
        fileName = depsDict["fileName"]

        if isDesps(fileName): # 已经下载安装好则跳过
            continue

        if action == "git": buildDeps(depsDict)
        if action == "gz": downloadAndBuild(depsDict)
        if action == "zip": downloadAndBuild(depsDict)
        if action == "bz2": downloadAndBuild(depsDict)

        updateDesps(fileName)
    pass


def genDepsCmakeList():
    log("-"*80)
    log("Generate Deps CmakeList in Path: " + homeDir)
    os.chdir(homeDir)

    libDir = os.path.join(outputDir, "lib")
    libs = getAllFileInDirectory(libDir)
    for lib in libs:
        if os.path.isdir(lib):
            continue
        sufix = os.path.splitext(lib)[-1]
        if sufix not in libSufixs:
            continue
        # log("lib: "+ lib)
        libPath = os.path.join(libDir, lib)
        global cmakeOther
        # cmakeOther = cmakeOther + "\n" + "link_libraries(\"" + libPath + "\")"
        cmakeOther = cmakeOther + "\n" + "list(APPEND DEPS_LIBS \"" + libPath + "\")"

    depsCamke = "deps.cmake"

    depsSource = """
file(GLOB_RECURSE Deps_Source
    "depsSource/**/*.c"
    "depsSource/**/*.cc"
    "depsSource/**/*.h"
    "depsSource/**/*.hpp"
    "depsSource/**/*.h++"
    "depsSource/**/*.asm"
)
sourceGroup("" ${DepsSource})
set_source_files_properties(${DepsSource} PROPERTIES HEADER_FILE_ONLY TRUE)
"""
    if not depsSourceFlag: 
        depsSource = ""

    depsContent = """
message("This is deps.cmake")

set(deps_list pthread dl)

set(DEPS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/"""+outputDirName+"""/include/")
set(DEPS_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/"""+outputDirName+"""/lib/")

message("Deps Include Directory: ${DEPS_INCLUDE_DIR}")
message("Deps Lib Directory: ${DEPS_LIB_DIR}")

include_directories("${DEPS_INCLUDE_DIR}")
link_directories("${DEPS_LIB_DIR}")
file(GLOB_RECURSE Deps_Include ${DEPS_INCLUDE_DIR}**)

""" + depsSource + cmakeOther
    log("Deps CmakeList content: " + depsContent)
    with open(depsCamke, "w") as fileHandler:
        fileHandler.write(depsContent)
    pass


def genDirs():
    if not os.path.exists(sourceDirName):
        os.makedirs(sourceDirName)
    if not os.path.exists(outputDirName):
        os.makedirs(outputDirName)
    if not os.path.exists(thirdPartyDirName):
        os.makedirs(thirdPartyDirName)
    
    global homeDir, sourceDir, thirdPartyDir, outputDir

    homeDir = sys.path[0]
    log("Home Directory: " + homeDir)
    sourceDir = os.path.join(homeDir, sourceDirName)
    log("Deps Directory: " + sourceDir)
    thirdPartyDir = os.path.join(homeDir, thirdPartyDirName)
    log("ThirdParty Directory: " + thirdPartyDir)
    outputDir = os.path.join(homeDir, outputDirName)
    log("Install Directory: " + outputDir)
    # log("-"*80)

    PATH = outputDir
    PATH = PATH + ":" + outputDir + os.path.sep + "bin"
    PATH = PATH + ":" + outputDir + os.path.sep + "include"
    PATH = PATH + ":" + outputDir + os.path.sep + "lib"
    log("PATH:" + PATH)
    os.environ["PATH"] = os.getenv("PATH") + ":" + PATH
    # operator("echo $PATH")
    operator("which nasm")
    pass



if __name__ == '__main__':
    begin = datetime.datetime.now()
    log("更新时间：" + str(begin))

    # 生成目录
    genDirs()

    # 构建第三方库
    buildFromDepsFile()

    # # 构建本地第三方库
    buildThirdParty()

    # # 生成cmake文件
    genDepsCmakeList()

    end = datetime.datetime.now()
    log(('花费时间: %.3f 秒' % (end - begin).seconds))
    logRecord()
    pass




