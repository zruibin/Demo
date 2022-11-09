# QTWithCMakeDemo

# 介绍
#### 使用cmake管理qt项目的一个演示示例
#### 支持翻译文件
#### 支持资源文件
#### 支持exe图标设置
#### 头文件 源文件 ui文件分别放在对应目录下面而不是放在一个目录

# 编译工程
## 1 安装Cmake, Qt，编译器 
### 1.1 cmake：建议安装最新版本的cmake工具
### 1.2 Qt：建议安装qt5.6及以上版本，qt6也可以

### 1.3 编译器：安装和qt版本对应的编译器
#### 1、比如说安装了qt5.6.3 MSVC 2013 32-bit,则需要安装visual studio 2013
#### 2、比如说安装了qt5.15.1 MSVC 2019 64-bit,则需要安装visual studio 2019
#### 3、比如说安装了qt5.15.1 MinGW 8.1.0 32-bit,则需要安装MinGW 8.1.0编译器

## 2 选择编译方式
### 2.1 使用Qtcreator4.13及以上版本打开CMakeLists.txt
#### qtcreator主界面，open,选择CMakeLists.txt即可打开本项目
### 2.2 使用vscode打开项目文件夹（需要vscode安装相应的插件）
#### 安装vscode以及相关插件（C/C++， CMake, formatter等插件） 
#### 安装完插件后的vscode底部状态栏可以选择编译器(编译器可以自动检测到)，编译方式（debug/release）等等功能
### 2.3 使用visual studio打开
#### 1、windows上需要安装带有gui的cmake，使用带界面的cmake生成***.sln项目工程文件
#### 2、安装了VS2017及以上版本的VS IDE,可以直接打开CMakeLists.txt


# 问题

[building for macOS-x86_64 but attempting to link with file built for macOS-arm64](https://zhuanlan.zhihu.com/p/348532259)

编译项目的时候，无脑cmake ..

> ld: warning: ignoring file /opt/homebrew/lib/libopencv_xxx.dylib building for macOS-x86_64 but attempting to link with file built for macOS-arm64

马上就会出现这个错误，原因在于cmake的配置，使得的是x86_64版本，需要编译的时候加入两个设置：

> cmake -DCMAKE_SYSTEM_PROCESSOR=arm64 -DCMAKE_OSX_ARCHITECTURES=arm64 ..

然后才能正确编译出你所需要的arm64版本的文件，运行和原来没有任何区别


# 教程

* [Qt6系列教程汇总](https://blog.csdn.net/dengjin20104042056/article/details/115174639)
* [QT6基本控件入门--Apple的学习笔记](https://www.jianshu.com/p/37048a1cc34e)
* [QT6小项目进阶准备--Apple的学习笔记](https://www.jianshu.com/p/1009df36bfce)

