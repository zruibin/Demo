# QtDemo

## 介绍

### 使用cmake管理qt项目的一个演示示例

## 编译工程

### 1 安装Cmake, Qt，编译器 
#### 1.1 cmake：建议安装最新版本的cmake工具
#### 1.2 Qt：qt6.2及以上

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

* [QT Multimedia](https://github.com/qt/qtmultimedia.git)

### Qt GUI从简单到复杂

 * https://gitee.com/zruibin/qt_gui_simple2complex

### QT布局

* http://c.biancheng.net/view/9422.html
* https://blog.csdn.net/zhangchuan7758/article/details/122428071
* https://blog.csdn.net/llk15884975173/article/details/120323353

### 动画

* http://t.zoukankan.com/itrena-p-5938274.html

### QSS

* http://www.skcircle.com/?id=971

### Permission

* https://doc-snapshots.qt.io/qt6-dev/qpermission.html
* https://blog.csdn.net/aggs1990/article/details/128603440
