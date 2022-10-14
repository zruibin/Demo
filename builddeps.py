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

import os, re, json, sys, platform
import subprocess, shutil
import datetime

homeDir = ""
sourceDir = ""
outputDir = ""
sourceDirName = "depsSource"
outputDirName = "deps"
buildDir = "buildGen"

cmakeFlags = ""

slash = "/"
if(platform.system()=='Windows'):
    slash = "\\"

logList = []

def logRecord():
    with open('builddeps.log', 'w') as fileHandle:
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


def buildDeps(dirStr, gitUrl, cloneList, cmdList, genBuilding=True, preCmdList=[]):
    if len(dirStr) == 0 and len(gitUrl) == 0 and len(cloneList) == 0 and len(cmakeList) == 0:
        log("Building Deps Was Error!")
        return
    log("-"*80)
    log("Start Building Deps: " + dirStr)
    os.chdir(sourceDir) #进入到源代码存放的目录
    operatorCMD(cloneList)
    os.chdir(dirStr)
    if genBuilding:
        if os.path.exists(buildDir):
            shutil.rmtree(buildDir)
        os.makedirs(buildDir)
        os.chdir(buildDir)
    log("当前编译路径：" + os.getcwd())
    if len(preCmdList) > 0:
        operatorCMD(preCmdList, False)    
    operatorCMD(cmdList, False)
    operator("make", False)
    operator("make install", False)
    pass

def buildDepsByList(buildDepsList, genBuilding=True, preCmdList=[]):
    if len(buildDepsList) < 4:
        log("Building Deps List Was Error!")
        return
    buildDeps(buildDepsList[0], buildDepsList[1], buildDepsList[2], buildDepsList[3], genBuilding, preCmdList)
    pass

def genDepsCmakeList():
    log("-"*80)
    log("Generate Deps CmakeList in Path: " + homeDir)
    os.chdir(homeDir)
    depsCamke = "deps.cmake"
    depsContent = """
message("This is deps.cmake")

set(deps_list pthread dl)

set(DEPS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/"""+outputDirName+"""/include")
set(DEPS_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/"""+outputDirName+"""/lib")

message("Deps Include Directory: ${DEPS_INCLUDE_DIR}")
message("Deps Lib Directory: ${DEPS_LIB_DIR}")

include_directories("${DEPS_INCLUDE_DIR}")
link_directories("${DEPS_LIB_DIR}")

"""+flags+"""

"""
    log("Deps CmakeList content: " + depsContent)
    with open(depsCamke, "w") as fileHandler:
        fileHandler.write(depsContent)
    pass


if __name__ == '__main__':
    begin = datetime.datetime.now()
    log("更新时间：" + str(begin))

    if not os.path.exists(sourceDirName):
        os.makedirs(sourceDirName)
    if not os.path.exists(outputDirName):
        os.makedirs(outputDirName)
    
    homeDir = sys.path[0]
    log("Home Directory: " + homeDir)
    sourceDir = homeDir + slash + sourceDirName
    log("Deps Directory: " + sourceDir)
    outputDir = homeDir + slash + outputDirName
    log("Install Directory: " + outputDir)

    abseilList = [
        "abseil",
        "https://github.com/abseil/abseil-cpp",
        ["git", "clone", "-b", "20211102.0", "--depth=1", "https://github.com/abseil/abseil-cpp", "abseil"],
        ["cmake", "-D CMAKE_BUILD_TYPE=RELEASE", 
            "-DABSL_BUILD_TESTING=ON -DABSL_USE_GOOGLETEST_HEAD=OFF -DCMAKE_CXX_STANDARD=11", 
            "-D", "CMAKE_INSTALL_PREFIX="+outputDir, ".."]
    ]
    # buildDepsByList(abseilList)

    wxWidgetsList = [
        "wxWidgets",
        "https://github.com/wxWidgets/wxWidgets.git",
        ["git", "clone", "--recurse-submodules", "-b", "v3.2.1", "--depth=1", "https://github.com/wxWidgets/wxWidgets", "wxWidgets"],

        ["../configure", "--disable-shared", "--enable-stl", 
            "--with-libtiff=builtin", "--with-libjpeg=builtin", "--with-libpng=builtin", "--with-zlib=builtin", 
            "--with-libiconv", "--with-opengl", "--with-regex", "--with-expat", "--with-libnotify",
            "--without-subdirs",
            # "--enable-sockets", "--enable-ipc", "--enable-datetime", "--enable-file", "--enable-filesystem", 
            # "--enable-fontenum", "--enable-fontmap", "--enable-fswatcher", "--enable-sound", "--enable-timer", "--enable-url"，
            # "--enable-svg",
            "--prefix="+outputDir],

        # ["cmake",  "-D CMAKE_BUILD_TYPE=RELEASE", '-G "Ninja"', 
        #     "-D wxUSE_GUI=ON",
        #     "-D wxBUILD_SHARED=OFF", "-D wxUSE_STL=ON", "-D wxUSE_EXPAT=builtin", 
        #     "-D wxUSE_LIBLZMA=builtin", "-D wxUSE_EXPAT=builtin",
        #     # "-D wxBUILD_MONOLITHIC=ON" #Build a single library
        #     "-D CMAKE_INSTALL_PREFIX="+outputDir, ".."]
        # ["cmake --build . --target install"],
    ]
    # https://blog.csdn.net/davidullua/article/details/126755300
    # ios https://docs.wxwidgets.org/3.2/plat_ios_install.html
    # buildDepsByList(wxWidgetsList)
    # wxWidgetsConfig = outputDir + "/bin/wx-config --libs --cppflags"
    # flags = os.popen(wxWidgetsConfig).readlines()
    # if len(flags) > 0:
    #     cmakeFlags = 'list(APPEND flags'
    #     log(wxWidgetsConfig, True)
    #     for flag in flags:
    #         for fg in flag.split(" "):
    #             if len(fg.strip()) > 0 :
    #                 cmakeFlags = cmakeFlags + ' "' + fg.strip() + '"'
    #     cmakeFlags = cmakeFlags + ")"
    #     log(cmakeFlags, True)

  
    genDepsCmakeList()
    end = datetime.datetime.now()
    log(('花费时间: %.3f 秒' % (end - begin).seconds))
    logRecord()
    pass





