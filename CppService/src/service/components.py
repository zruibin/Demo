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
import datetime, shutil


loginName = os.getlogin()
today = datetime.datetime.now()
todayStr = today.strftime("%Y/%m/%d")
year = today.year


componentName = "components"
filePath = os.path.split(os.path.realpath(__file__))[0]
componentPath = os.path.join(filePath, componentName)
shellName = "shell"
shellPath = os.path.join(filePath, shellName)
factoryName = "ComponentFactory.cc"
factoryFile = os.path.join(shellPath, factoryName)

# print("filePath: " + filePath)
# print("componentPath: " + componentPath)
# print("shellPath: " + shellPath)
# print("factoryFile: " + factoryFile)

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

class ${name} : public BaseService
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

class ${name} : public BaseServiceAdapter
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
    std::string Description() override;
    
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
#include <memory>
#include "BaseServiceEngine.h"

void ${name}::Init() {

}

void ${name}::Destory() {
    
}

void ${name}::SetAdapter(const std::shared_ptr<BaseServiceAdapter> adapter) {
    if (adapter == nullptr) {
        return;
    }
    adapter_ = adapter->GetCastAdapter<${adapterName}>();
}

std::string ${name}::Description() {
    return "${name}";
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

class ${name} : public ${adapterInterfaceName}
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
#include "BaseServiceEngine.h"
#include "BaseService.h"
#include "${implName}.h"
#include "${adapterName}.hpp"

class ${name} : public BaseServiceBuilder
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
    
    std::string GetBuilderName() override {
        return "${name}";
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


def deleteComponent(name):
    deletePaths = [
        os.path.join(componentPath, name), 
        os.path.join(shellPath, name + "Builder")
    ]
    for path in deletePaths:
        if os.path.exists(path):
            log(path)
            shutil.rmtree(path)
    pass


def updateComponentFactory():
    existComponents = Path(shellPath)
    existComponents = [x for x in existComponents.iterdir() if x.is_dir()]

    headers = []
    builders = []
    for path in existComponents:
        headers.append('#include "' + path.name + '.hpp"')
        builders.append("    AddBuilder<" + path.name + ">();")
    headersString = "\n".join(headers)
    buildersString = "\n".join(builders)
    # log(headersString)
    # log(buildersString)

    factoryContent = """/*
 * ${factoryName}
 *
 * Created by Ruibin.Chow on 2023/07/12.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "ComponentFactory.h"
${headersString}

void ComponentFactory::ConstructBuilders() {
${buildersString}
}
"""
    factoryContent = TPL(factoryContent).substitute({
                                                "factoryName": factoryName,
                                                "headersString": headersString,
                                                "buildersString": buildersString
                                                })

    with open(factoryFile, "w") as fileHandle:
            fileHandle.write(str(factoryContent))

    pass


def help():
    helpStr = """
Command:
    update/-p         根据components目录中的组件生成${factoryName}内容
    create/-c name    根据name生成组件
    help/-h           说明
Default:
    update
    """
    helpStr = TPL(helpStr).substitute({"factoryName": factoryName})
    log(helpStr)
    pass


if __name__ == '__main__':
    if len(sys.argv) == 1:
        updateComponentFactory()
    if len(sys.argv) > 1:
        action = str(sys.argv[1])
        if action == "help" or action == "-h":
            help()
        elif action == "create" or action == "-c":
            if len(sys.argv) > 2:
                name = str(sys.argv[2])
                if check(name):
                    createComponent(name)
                    updateComponentFactory()
            else:
                log("Please input component name.")
        elif action == "delete" or action == "-d":
            if len(sys.argv) > 2:
                name = str(sys.argv[2])
                deleteComponent(name)
                updateComponentFactory()
            else:
                log("Please input component name.")
        else:
            help()
    pass





