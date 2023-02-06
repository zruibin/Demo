#! /usr/bin/env python3
# -*- coding: utf-8 -*- 
#
# generateDep.py
#
# Created by Ruibin.Chow on 2023/02/06.
# Copyright (c) 2023年 Ruibin.Chow All rights reserved.
# 

"""
https://www.cnblogs.com/ping-y/p/5897018.html
"""

import os, re, json, sys, platform, fnmatch, stat
import datetime
from pathlib import Path


depsCamke = "deps.cmake"
cmakeList = "CMakeLists.txt"
configure = "configure"
makefile = "Makefile"

content = r"""

cmake_minimum_required (VERSION 3.0)

message(WARNING "This Target(PROJECT_NAME) Create By Ruibin.Chow.")

project(PROJECT_NAME)

set(TARGET_EXE "PROJECT_NAME_exe")
set(TARGET_BUILD "PROJECT_NAME_building")


set(sourceGroupPrefixName "")

# 按文件层次结构显示
function(sourceGroup prefix)
    foreach(FILE IN LISTS ARGN) 
        get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
        string(REPLACE "${prefix}" "" GROUP "${GROUP}")
        string(REPLACE "/" "\\" GROUP "${GROUP}")

        if ("${FILE}" MATCHES ".*\\.cpp" AND ".*\\.cc" AND ".*\\.c")
            set(GROUP "Source Files${GROUP}")
        elseif("${FILE}" MATCHES ".*\\.h" AND ".*\\.hpp")
            set(GROUP "Header Files${GROUP}")
        endif()

        source_group("${GROUP}" FILES "${FILE}")
    endforeach()
endfunction()


file(GLOB_RECURSE PROJECT_NAME_Source
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.h++"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.asm"
)
sourceGroup("" ${PROJECT_NAME_Source})
#message("sources: ${PROJECT_NAME_Source}")


add_executable(${TARGET_EXE} ${PROJECT_NAME_Source})

add_custom_target(${TARGET_BUILD}
    COMMAND ROOT_DIR=/path/
    COMMAND export PATH=$PATH::$ROOT_DIR/deps:$ROOT_DIR/deps/bin:$ROOT_DIR/deps/include:$ROOT_DIR/deps/lib
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND pwd
    COMMAND ARCH make
    COMMAND ARCH make install
    COMMAND echo "${TARGET_BUILD} done."
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

"""

"""
-------------------------------------------
-------------------------------------------
字体色     |       背景色     |      颜色描述
-------------------------------------------
30        |        40       |       黑色
31        |        41       |       红色
32        |        42       |       绿色
33        |        43       |       黃色
34        |        44       |       蓝色
35        |        45       |       紫红色
36        |        46       |       青蓝色
37        |        47       |       白色
--------------------------------------------------------------------------显示方式     |      效果
-------------------------------
0           |     终端默认设置
1           |     高亮显示
4           |     使用下划线
5           |     闪烁
7           |     反白显示
8           |     不可见-------------------------------
"""
def log(string="", color=None, newline=True):
    if color != None:
        string = "\033[" + str(color) + "m" + string + "\033[0m"
    if newline:
        print(string, end="\n")
    else:
        print(string, end="")
    pass


def main(path):
    homeDir = sys.path[0]
    log("Home Directory: " + homeDir)
    sourceDir = os.path.join(homeDir, path)
    log("sourceDir: " + sourceDir)

    name = os.path.basename(sourceDir)

    global depsCamke, cmakeList, configure, makefile
    depsCamke = os.path.join(homeDir, depsCamke)
    cmakeList = os.path.join(sourceDir, cmakeList)
    configure = os.path.join(sourceDir, configure)
    makefile = os.path.join(sourceDir, makefile)
    log("depsCamke: " + depsCamke, color=33)
    log("cmakeList: " + cmakeList, color=33)
    log("configure: " + configure, color=33)
    log("makefile: " + makefile, color=33)

    if not os.path.exists(depsCamke):
        log(os.path.basename(depsCamke) + " was not exist!", color=31)
        return

    if not os.path.exists(configure):
        log(os.path.basename(configure) + " was not exist!", color=31)
        return

    if not os.path.exists(makefile):
        log(os.path.basename(makefile) + " was not exist!", color=31)
        return

    if os.path.exists(cmakeList):
        log(os.path.basename(cmakeList) + " was exist!", color=31)
        return

    global content
    content = content.replace("PROJECT_NAME", name)
    content = content.replace("/path/", homeDir)

    arch = ""
    if platform.machine() == "arm64" and platform.system() == "Darwin":
        arch = "arch -arm64"
    content = content.replace("ARCH", arch)
    
    log(content)

    with open(cmakeList, "w") as fileHandle:
        fileHandle.write(str(content))

    depsData = """

add_subdirectory(%s)
list(APPEND Deps_Source_Targets %s_building)

    """
    depsData = depsData % (sourceDir, name)
    
    with open(depsCamke, "a+") as fileHandle:
        fileHandle.write(str(depsData))

    pass


if __name__ == '__main__':
    if len(sys.argv) > 1:
        path = sys.argv[1]
        main(path)
    else:
        print("\033[5;31m" + "Error: It was need path!" + "\033[0m")
    pass






