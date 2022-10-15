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
import subprocess, shutil, os.path
import datetime

homeDir = ""
sourceDir = ""
outputDir = ""
sourceDirName = "depsSource"
outputDirName = "deps"
buildDir = "buildGen"

cmakeOther = ""

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


def buildDeps(dirStr, gitUrl, cloneList, cmdList, genBuilding=True, preCmdList=[], install=True):
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
    print("preCmdList: "+str(len(preCmdList)))
    if len(preCmdList) > 0:
        
        operatorCMD(preCmdList, False)    
    operatorCMD(cmdList, False)
    if install:
        operator("cmake --build . --target install", False)
    pass

def buildDepsByList(buildDepsList, preCmdList=[], genBuilding=True, install=True):
    if len(buildDepsList) < 4:
        log("Building Deps List Was Error!")
        return
    buildDeps(buildDepsList[0], buildDepsList[1], buildDepsList[2], buildDepsList[3], genBuilding, preCmdList, install)
    pass

def genLinkLibraries():
    libDir = os.path.join(outputDir, "lib")
    libs = os.listdir(libDir)
    global cmakeOther
    for lib in libs:
        if os.path.isdir(lib):
            continue
        log("lib: "+lib)
        libPath = os.path.join(libDir, lib)
        cmakeOther = cmakeOther + "\n" + "link_libraries(\"" + libPath + "\")"
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

"""+cmakeOther

    log("Deps CmakeList content: " + depsContent)
    with open(depsCamke, "w") as fileHandler:
        fileHandler.write(depsContent)
    pass


def wxWidgetsMacConfigs(libDir):
    WXTOOLKITUPPER = "COCOA"
    WXTOOLKIT = "cocoa"
    WXROOT = os.path.join(sourceDir, "wxWidgets")
    WXPLATFORM = "__WXOSX_"+WXTOOLKITUPPER+"__"
    WX_PREPROCESSOR_DEFINITIONS = [
        "WXBUILDING",
        WXPLATFORM, "MACOS_CLASSIC", "__WXMAC_XCODE__",
        "__WX__", "_LARGE_FILES", "SCI_LEXER",
        "NO_CXX11_REGEX", "WX_PRECOMP",
        "wxUSE_UNICODE_WCHAR", "HAVE_CONFIG_H", "PNG_NO_CONFIG_H",
        "_FILE_OFFSET_BITS=64", "wxUSE_UNICODE_UTF8=0",
    ]
    GCC_PREPROCESSOR_DEFINITIONS = " ".join(WX_PREPROCESSOR_DEFINITIONS)

    USER_HEADER_SEARCH_PATHS = [
        WXROOT + "/include",
        WXROOT + "/build/osx/setup/" + WXTOOLKIT + "/include",
        WXROOT + "/src/zlib",
        WXROOT + "/src/jpeg",
        WXROOT + "/src/png",
        WXROOT + "/src/expat/expat/lib",
        WXROOT + "/src/tiff/libtiff",
        WXROOT + "/src/stc/scintilla/src",
        WXROOT + "/src/stc/scintilla/include",
        WXROOT + "/src/stc/scintilla/lexlib",
        WXROOT + "/3rdparty/pcre/src/wx",
    ]
    HEADER_SEARCH_PATHS = [
        WXROOT + "/src/tiff/libtiff",
        WXROOT + "/src/regex",
        WXROOT + "/3rdparty/pcre/src/wx"
    ]
    OTHER_LDFLAGS = [
        "-framework WebKit", 
        "-framework IOKit",
        "-framework Carbon", 
        "-framework Cocoa", 
        "-framework AudioToolbox", 
        "-framework OpenGL", 
        "-framework AVFoundation", 
        "-framework CoreMedia",
        "-framework Security", 
        "-framework QuartzCore", 
        "-weak_framework AVKit"
    ]

    configDict = {
        "WXPLATFORM": WXPLATFORM,
        "WXROOT": WXROOT,
        "WXTOOLKIT": WXTOOLKIT,
        "WXTOOLKITUPPER": WXTOOLKITUPPER,
        "WX_PREPROCESSOR_DEFINITIONS": " ".join(WX_PREPROCESSOR_DEFINITIONS),

        "GCC_PREPROCESSOR_DEFINITIONS": GCC_PREPROCESSOR_DEFINITIONS,
        "USER_HEADER_SEARCH_PATHS": " ".join(USER_HEADER_SEARCH_PATHS),
        "HEADER_SEARCH_PATHS": " ".join(HEADER_SEARCH_PATHS),
        "OTHER_LDFLAGS": " ".join(OTHER_LDFLAGS),
    }
    for k,v in configDict.items():
        log(k+": "+v)

    global cmakeOther
    for item in WX_PREPROCESSOR_DEFINITIONS:
        cmakeOther = cmakeOther + "\n" + "add_definitions(-D" + item + ")"
    for item in HEADER_SEARCH_PATHS:
        cmakeOther = cmakeOther + "\n" + "include_directories(\"" + item + "\")"
    for item in USER_HEADER_SEARCH_PATHS:
        cmakeOther = cmakeOther + "\n" + "include_directories(\"" + item + "\")"
    for item in OTHER_LDFLAGS:
        cmakeOther = cmakeOther + "\n" + "list(APPEND deps_list \"" + item + "\")"

    libs = os.listdir(libDir)
    for lib in libs:
        log("lib: "+lib)
        libPath = os.path.join(libDir, lib)
        # cmakeOther = cmakeOther + "\n" + "list(APPEND deps_list \"" + libPath + "\")"
        cmakeOther = cmakeOther + "\n" + "link_libraries(\"" + libPath + "\")"
    pass

def wxWidgetsMacBuiding():
    WX_ROOT = os.path.join(sourceDir, "wxWidgets")
    WX_MAC_ROOT = os.path.join(WX_ROOT, "build", "osx")
    project = os.path.join(WX_MAC_ROOT, "wxcocoa.xcodeproj")
    scheme = "static" # dynamic or static, error for dynamic
    configuration = "Release" # release or debug
    outputDir = os.path.join(WX_ROOT, buildDir, "output_"+scheme)
    if not os.path.exists(outputDir):
        os.makedirs(outputDir)
    product = os.path.join(outputDir, "Build", "Products", configuration)
    cmdStr = "xcodebuild -project %s -scheme %s -configuration %s -derivedDataPath %s" % (project, scheme, configuration, outputDir)
    log("build Command: "+cmdStr)
    log("product dir: "+product)
    return cmdStr, product



if __name__ == '__main__':
    begin = datetime.datetime.now()
    log("更新时间：" + str(begin))

    if not os.path.exists(sourceDirName):
        os.makedirs(sourceDirName)
    if not os.path.exists(outputDirName):
        os.makedirs(outputDirName)
    
    homeDir = sys.path[0]
    log("Home Directory: " + homeDir)
    sourceDir = os.path.join(homeDir, sourceDirName)
    log("Deps Directory: " + sourceDir)
    outputDir = os.path.join(homeDir, outputDirName)
    log("Install Directory: " + outputDir)

    abseilList = [
        "abseil",
        "https://github.com/abseil/abseil-cpp",
        ["git", "clone", "-b", "20211102.0", "--depth=1", "https://github.com/abseil/abseil-cpp", "abseil"],
        ["cmake", "-D CMAKE_BUILD_TYPE=RELEASE", 
            "-DABSL_BUILD_TESTING=ON -DABSL_USE_GOOGLETEST_HEAD=OFF -DCMAKE_CXX_STANDARD=11", 
            "-D", "CMAKE_INSTALL_PREFIX="+outputDir, ".."]
    ]
    buildDepsByList(abseilList)

    cmdStr, product = wxWidgetsMacBuiding()
    wxWidgetsList = [
        "wxWidgets",
        "https://github.com/wxWidgets/wxWidgets.git",
        ["git", "clone", "--recurse-submodules", "-b", "v3.2.1", "--depth=1", "https://github.com/wxWidgets/wxWidgets", "wxWidgets"],

        # ["../configure", "--disable-shared", "--enable-stl", 
        #     "--with-libtiff=builtin", "--with-libjpeg=builtin", "--with-libpng=builtin", "--with-zlib=builtin", 
        #     "--with-libiconv", "--with-opengl", "--with-regex", "--with-expat", "--with-libnotify",
        #     "--without-subdirs",
        #     # "--enable-sockets", "--enable-ipc", "--enable-datetime", "--enable-file", "--enable-filesystem", 
        #     # "--enable-fontenum", "--enable-fontmap", "--enable-fswatcher", "--enable-sound", "--enable-timer", "--enable-url"，
        #     # "--enable-svg",
        #     "--prefix="+outputDir],

        # ["cmake",  "-D CMAKE_BUILD_TYPE=RELEASE", #'-G "Ninja"', 
        #     "-D wxUSE_GUI=ON",
        #     "-D wxBUILD_SHARED=OFF", "-D wxUSE_STL=ON", "-D wxUSE_EXPAT=builtin", 
        #     "-D wxUSE_LIBLZMA=builtin", "-D wxUSE_EXPAT=builtin",
        #     "-D CMAKE_INSTALL_PREFIX="+outputDir, ".."]

        [cmdStr],
    ]
    # https://blog.csdn.net/davidullua/article/details/126755300
    # ios https://docs.wxwidgets.org/3.2/plat_ios_install.html
    buildDepsByList(wxWidgetsList, genBuilding=False, install=False)
    wxWidgetsMacConfigs(product)

    genLinkLibraries()
    genDepsCmakeList()
    end = datetime.datetime.now()
    log(('花费时间: %.3f 秒' % (end - begin).seconds))
    logRecord()
    pass




