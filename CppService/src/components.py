#! /usr/bin/env python
# -*- coding: utf-8 -*- 
#
# components.py
#
# Created by Ruibin.Chow on 2023/07/12.
# Copyright (c) 2023年 Ruibin.Chow All rights reserved.
# 

"""

"""

import os, re, json, sys, platform, fnmatch, stat
from pathlib import Path
from string import Template as TPL
import datetime


loginName = os.getlogin()
today = datetime.datetime.now()
todayStr = today.strftime("%Y/%m/%d")
year = today.year


componentName = "components"
filePath = os.path.split(os.path.realpath(__file__))[0]
componentPath = os.path.join(filePath, componentName)
shellName = "shell"
shellPath = os.path.join(filePath, shellName)
factoryFile = os.path.join(shellPath, "ComponentFactory.cc")
componentCmakeFile = os.path.join(filePath, "components.cmake")

# print("filePath: " + filePath)
# print("componentPath: " + componentPath)
# print("shellPath: " + shellPath)
# print("factoryFile: " + factoryFile)
# print("componentCmakeFile: " + componentCmakeFile)

componentIncludeName = "include"
componentSourceName = "source"

def log(string):
    print(str(string))

def check(name):
    existComponents = os.listdir(componentPath);
    # print(existComponents)
    if name in existComponents:
        log(str(name) + " was exist.")
        return False
    if not os.path.exists(factoryFile):
        return False
    return True


def createComponent(name):
    componentIncludePath = os.path.join(componentPath, name, componentIncludeName)
    componentSourcePath = os.path.join(componentPath, name, componentSourceName)
    log(componentIncludePath)
    log(componentSourcePath)
    if not os.path.exists(componentIncludePath):
        os.makedirs(componentIncludePath)
    if not os.path.exists(componentSourcePath):
        os.makedirs(componentSourcePath)

#-------------------------------------------------------------------------------

    interfaceName = name + "Interface"
    inferfacePath = os.path.join(componentIncludePath, interfaceName + ".h")
    # log(inferfacePath)
    interfaceContent = """/*
 * ${name}.h
 *
 * Created by ${loginName} on ${todayStr}.
 * Copyright (c) ${year}年 ${loginName} All rights reserved.
 */

#ifndef ${upperName}_H
#define ${upperName}_H

#include <memory>
#include "BaseService.h"

class ${name}: public BaseService
{
public:
    virtual ~${name}() = default;
};

#endif /* !${upperName}_H */"""
    interfaceContent = TPL(interfaceContent).substitute({
                                                "name": interfaceName,
                                                "upperName": interfaceName.upper(),
                                                "loginName": loginName,
                                                "todayStr": todayStr,
                                                "year": year
                                                })
    with open(inferfacePath, "w") as fileHandle:
            fileHandle.write(str(interfaceContent))

#-------------------------------------------------------------------------------

    interfaceAdapterName = name + "AdapterInterface"
    interfaceAdapterPath = os.path.join(componentIncludePath, interfaceAdapterName + ".h")
    log(interfaceAdapterPath)
    interfaceAdapterContent = """/*
 * ${name}.h
 *
 * Created by ${loginName} on ${todayStr}.
 * Copyright (c) ${year}年 ${loginName} All rights reserved.
 */

#ifndef ${upperName}_H
#define ${upperName}_H

#include "BaseServiceAdapter.h"

class ${name}: public BaseServiceAdapter
{
public:
    virtual ~${name}() = default;
};

#endif /* !${upperName}_H */"""
    interfaceAdapterContent = TPL(interfaceAdapterContent).substitute({
                                                        "name": interfaceAdapterName,
                                                        "upperName": interfaceAdapterName.upper(),
                                                        "loginName": loginName,
                                                        "todayStr": todayStr,
                                                        "year": year
                                                        })
    with open(interfaceAdapterPath, "w") as fileHandle:
            fileHandle.write(str(interfaceAdapterContent))

#-------------------------------------------------------------------------------

    implName = name + "Impl"
    headerImplPath = os.path.join(componentSourcePath, implName + ".h")
    # log(headerImplPath)
    headerImplContent = """/*
 * ${name}.h
 *
 * Created by ${loginName} on ${todayStr}.
 * Copyright (c) ${year}年 ${loginName} All rights reserved.
 */

#ifndef ${upperName}_H
#define ${upperName}_H

#include "${interfaceName}.h"
#include "${adapterName}.h"

class ${name} : public ${interfaceName}
{
public:
    explicit ${name}() = default;
    virtual ~${name}() = default;
    
public:
    void Init() override;
    void Destory() override;
    void SetAdapter(const std::shared_ptr<BaseServiceAdapter>) override;
    void Description() override;
    
private:
    std::shared_ptr<${adapterName}> adapter_;
};

#endif /* !${upperName}_H */"""
    headerImplContent = TPL(headerImplContent).substitute({
                                                "name": implName,
                                                "upperName": implName.upper(),
                                                "interfaceName": interfaceName,
                                                "adapterName": interfaceAdapterName,
                                                "loginName": loginName,
                                                "todayStr": todayStr,
                                                "year": year
                                                })
    with open(headerImplPath, "w") as fileHandle:
            fileHandle.write(str(headerImplContent))

#-------------------------------------------------------------------------------    

    sourceImplPath = os.path.join(componentSourcePath, implName + ".cc")
    # log(sourceImplPath)
    sourceImplContent = """/*
 * ${name}.h
 *
 * Created by ${loginName} on ${todayStr}.
 * Copyright (c) ${year}年 ${loginName} All rights reserved.
 */

#include "${name}.h"
#include <iostream>
#include <memory>
#include "BaseEngine.h"

void ${name}::Init() {
    std::cout << "${name}::Init." << std::endl;
}

void ${name}::Destory() {
    std::cout << "${name}::Destory." << std::endl;
}

void ${name}::SetAdapter(const std::shared_ptr<BaseServiceAdapter> adapter) {
    if (adapter == nullptr) {
        return;
    }
    adapter_ = adapter->GetCastAdapter<${adapterName}>();
}

void ${name}::Description() {
    std::cout << "${name}::Description." << std::endl;
}
"""
    sourceImplContent = TPL(sourceImplContent).substitute({
                                                "name": implName,
                                                "upperName": implName.upper(),
                                                # "interfaceName": interfaceName,
                                                "adapterName": interfaceAdapterName,
                                                "loginName": loginName,
                                                "todayStr": todayStr,
                                                "year": year
                                                })
    with open(sourceImplPath, "w") as fileHandle:
            fileHandle.write(str(sourceImplContent))

#-------------------------------------------------------------------------------

    builderName = name + "Builder"
    builderPath = os.path.join(shellPath, builderName)
    if not os.path.exists(builderPath):
        os.makedirs(builderPath)

    adapterName = name + "AdapterImpl"
    adapterfile = os.path.join(builderPath, adapterName + ".hpp")
    # log(adapterfile)
    adapterContent = """/*
 * ${name}.h
 *
 * Created by ${loginName} on ${todayStr}.
 * Copyright (c) ${year}年 ${loginName} All rights reserved.
 */

#ifndef ${upperName}_H
#define ${upperName}_H

#include "${adapterInterfaceName}.h"

class ${name}: public ${adapterInterfaceName}
{
public:
    ${name}() = default;
    ~${name}() = default;
};

#endif /* !${upperName}_H */
"""
    adapterContent = TPL(adapterContent).substitute({
                                                "name": adapterName,
                                                "upperName": adapterName.upper(),
                                                "adapterInterfaceName": interfaceAdapterName,
                                                "loginName": loginName,
                                                "todayStr": todayStr,
                                                "year": year
                                                })
    with open(adapterfile, "w") as fileHandle:
            fileHandle.write(str(adapterContent))

#-------------------------------------------------------------------------------

    builderfile = os.path.join(builderPath, builderName + ".hpp")
    log(builderPath)
    builderContent = """/*
 * ${name}.h
 *
 * Created by ${loginName} on ${todayStr}.
 * Copyright (c) ${year}年 ${loginName} All rights reserved.
 */

#ifndef ${upperName}_H
#define ${upperName}_H

#include "BaseServiceBuilder.h"
#include "BaseEngine.h"
#include "BaseService.h"
#include "${implName}.h"
#include "${adapterName}.hpp"

class ${name}: public BaseServiceBuilder
{
public:
    ${name}() = default;
    virtual ~${name}() = default;

public:
    std::shared_ptr<BaseService> BuildService() override {
        std::shared_ptr<BaseServiceAdapter> adapter = std::make_shared<${adapterName}>();
        adapter->SetEngine(engine_.lock());
        std::shared_ptr<${interfaceName}> impl = std::make_shared<${implName}>();
        impl->SetAdapter(adapter);
        return impl;
    };

    std::string GetServiceName() override {
        return "${interfaceName}";
    };
};

#endif /* !${upperName}_H */
"""
    builderContent = TPL(builderContent).substitute({
                                                "name": builderName,
                                                "upperName": builderName.upper(),
                                                "interfaceName": interfaceName,
                                                "implName": implName,
                                                "adapterName": adapterName,
                                                "loginName": loginName,
                                                "todayStr": todayStr,
                                                "year": year
                                                })
    with open(builderfile, "w") as fileHandle:
            fileHandle.write(str(builderContent))

    pass


def updateComponentFactory():
    existComponents = Path(shellPath)
    existComponents = [x for x in existComponents.iterdir() if x.is_dir()]

    headers = []
    builders = []
    for path in existComponents:
        headers.append('#include "' + path.name + '.hpp"')
        builders.append("    ADD_SERVICE_BUILDER(" + path.name + ")")
    headersString = "\n".join(headers)
    buildersString = "\n".join(builders)
    # log(headersString)
    # log(buildersString)

    factoryContent = r"""/*
 * ComponentFactory.cc
 *
 * Created by Ruibin.Chow on 2023/07/12.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "ComponentFactory.h"
${headersString}

#define ADD_SERVICE_BUILDER(_builder_) { \
    std::shared_ptr<BaseServiceBuilder> builder = std::make_shared<_builder_>(); \
    builderMap_->emplace(builder->GetServiceName(), builder); \
}

void ComponentFactory::ConstructBuilders() {
${buildersString}
}
"""
    factoryContent = TPL(factoryContent).substitute({
                                                "headersString": headersString,
                                                "buildersString": buildersString
                                                })

    with open(factoryFile, "w") as fileHandle:
            fileHandle.write(str(factoryContent))

    pass


def updateCmake():
    existComponents = Path(componentPath)
    existComponents = [x for x in existComponents.iterdir() if x.is_dir()]

    directories = []
    for path in existComponents:
        directories.append("# " + path.name)
        directories.append("list(APPEND DIRECTORIES ${COMPONENT_PATH}/shell/" + path.name + "Builder)")
        directories.append("list(APPEND DIRECTORIES ${COMPONENT_PATH}/components/" + path.name + "/" + componentIncludeName + ")")
        directories.append("list(APPEND DIRECTORIES ${COMPONENT_PATH}/components/" + path.name + "/" + componentSourceName + ")")
        directories.append("list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENT_PATH}/components/" + path.name + "/" + componentIncludeName + ")")
    # log(directories)
    directoriesString = "\n".join(directories)

    cmakeContent = """
set(COMPONENT_PATH "${CMAKE_SOURCE_DIR}/src")

list(APPEND DIRECTORIES ${COMPONENT_PATH}/base)
list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENT_PATH}/base)
list(APPEND DIRECTORIES ${COMPONENT_PATH}/shell)
""" + directoriesString + """

foreach(directory IN LISTS DIRECTORIES)
    # 相当于g++选项中的-I参数的作用
    include_directories(${directory})

    file(GLOB_RECURSE files "${directory}/*.h")
    list(APPEND HEADERS "${files}")
    file(GLOB_RECURSE files "${directory}/*.hpp")
    list(APPEND HEADERS "${files}")
    
    file(GLOB_RECURSE files "${directory}/*.c")
    list(APPEND SOURCES "${files}")
    file(GLOB_RECURSE files "${directory}/*.cc")
    list(APPEND SOURCES "${files}")
    file(GLOB_RECURSE files "${directory}/*.cpp")
    list(APPEND SOURCES "${files}")
endforeach()

list(APPEND HEADERS "${COMPONENT_PATH}/Engine.h")
list(APPEND SOURCES "${COMPONENT_PATH}/Engine.cc")


list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENT_PATH}/base)
list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENT_PATH}/components/service/include)
foreach(directory IN LISTS EXPORT_HEADS_DIRECTORIES)
    file(GLOB_RECURSE files "${directory}/*.h")
    list(APPEND EXPORT_HEADS "${files}")
    file(GLOB_RECURSE files "${directory}/*.hpp")
    list(APPEND EXPORT_HEADS "${files}")
endforeach()
list(APPEND EXPORT_HEADS "${COMPONENT_PATH}/Engine.h")

list(APPEND RESOURCES "${COMPONENT_PATH}/components.cmake")
list(APPEND RESOURCES "${COMPONENT_PATH}/components.py")


# message(STATUS "HEADERS: ${HEADERS}")
# message(STATUS "SOURCES: ${SOURCES}")
# message(STATUS "EXPORT_HEADS: ${EXPORT_HEADS}")
# message(STATUS "RESOURCES: ${RESOURCES}")
    """
    with open(componentCmakeFile, "w") as fileHandle:
            fileHandle.write(str(cmakeContent))

    pass


if __name__ == '__main__':
    if len(sys.argv) == 1:
        log("Please input component name.")
    if len(sys.argv) == 2:
        name = str(sys.argv[1])
        if check(name):
            createComponent(name)
            updateComponentFactory()
            updateCmake()
    pass





