
## FFMpegDemo

* https://blog.csdn.net/qq_52703909/article/details/123672054
* https://www.cnblogs.com/yaoz/p/6944942.html
* https://blog.csdn.net/ternence_hsu/article/details/104157985
* https://blog.csdn.net/lidec/article/details/122531908

## cmake findpackage

* https://www.jianshu.com/p/a0915895dbbc
* https://www.jianshu.com/p/5dc0b1bc5b62

```
list(APPEND CMAKE_SYSTEM_PREFIX_PATH "${DEPS_LIB_DIR}/cmake/")
find_package(ogg REQUIRED)
list(APPEND DEPS_LIBS ogg)
```


## pkg-config

```
include(FindPkgConfig)
set(PKG_CONFIG_EXECUTABLE "${CMAKE_CURRENT_SOURCE_DIR}/deps/bin/pkg-config")
set(ENV{PKG_CONFIG_PATH} ${DEPS_LIB_DIR}/pkgconfig/)

file(GLOB_RECURSE PkgConfigFiles ${DEPS_LIB_DIR}pkgconfig/*.pc)
foreach(FILE ${PkgConfigFiles}) 
        get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
        get_filename_component(FILE_NAME "${FILE}" NAME)
        get_filename_component(FILE_NAME_WE "${FILE}" NAME_WE)

        string(REPLACE lib "" RESULT ${FILE_NAME_WE})
        message("RESULT: ${RESULT}, LIB: ${FILE_NAME_WE}")

        pkg_search_module(${FILE_NAME_WE} REQUIRED ${FILE_NAME_WE})
        list(APPEND DEPS_LIBS ${RESULT})

        message("FILE_NAME: ${FILE_NAME}, LIB: ${FILE_NAME_WE}")
        #message("PkgConfig include: ${${FILE_NAME_WE}_INCLUDE_DIRS}")
		#message("PkgConfig libs: ${${FILE_NAME_WE}_LIBRARIES}")
endforeach()
```


## 查看库是否是debug包

* 执行

```
dwarfdump -debug-info /path/FFMpegDemo/deps/lib/libavformat.a | head -n 100
```

* 输出

```
/path/FFMpegDemo/deps/lib/libavformat.a(3dostr.o):      file format Mach-O arm64

.debug_info contents:
0x00000000: Compile Unit: length = 0x00003493, format = DWARF32, version = 0x0004, abbr_offset = 0x0000, addr_size = 0x08 (next unit at 0x00003497)

0x0000000b: DW_TAG_compile_unit
              DW_AT_producer    ("Apple clang version 14.0.0 (clang-1400.0.29.102)")
              DW_AT_language    (DW_LANG_C99)
              DW_AT_name        ("src/libavformat/3dostr.c")
              DW_AT_LLVM_sysroot        ("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk")
              DW_AT_APPLE_sdk   ("MacOSX.sdk")
              DW_AT_stmt_list   (0x00000000)
              DW_AT_comp_dir    ("/path/FFMpegDemo/depsSource/ffmpeg-5.1.2/")
              DW_AT_low_pc      (0x0000000000000000)
              DW_AT_high_pc     (0x00000000000008d8)
```

其中：

```
DW_AT_comp_dir    ("/path/FFMpegDemo/depsSource/ffmpeg-5.1.2/")
```

## ffmpeg源码debug

### 一、cmake源码集成方式

> 注意：此方式存在函数跳转问题

#### **builddeps.py中 `IS_DEBUG` 设置为True即可，默认为True.**

#### OR

1. configure时加上**--extra-cflags=-g --extra-cflags=\"-fno-stack-check\" --enable-debug --disable-optimizations --disable-stripping**

> 注意：必须在configure所在的目录下执行与编译

2. 将源码文件目录以**Create folder references**的方式加进xocde中，即可断点调试

参考：https://blog.csdn.net/weixin_45581597/article/details/127728079

### 二、xcode工程加入方式

> builddeps.py中**depsSourceFlag**设置为False，防止引过多源码
>  
> 此方式必须是有加调试模式的configeure生成的Makefile，即**一.1**所述
>  
> 注意：此方式**不存在**函数跳转问题

1. 新建空的**Command Line Tool**工程(Language选C++)，创建成功后将main.cpp及其目录删除只留下.xcodeproj文件(如FFMpeg.xcodeproj)

2. 将FFMpeg.xcodeproj放于FFMpeg源码目录中，打开工程，将libavformat等源码目录拖进工程中，归属FFMpeg(**即加入FFMpeg的可运行target，此步是为了能够建立起文件索引以进行函数跳转**)

> 在将libavformat等源码目录拖进工程**前**最好先执行下make clean，因为源码目录中可能包含.o文件，以免造成文件过多

3. 新建名为FFMpeg_Building的Aggregate的target，在此target的**Build Phases**中新建脚本并执行以下脚本

```
pwd

# 需要将path替换为具体的所在路径
ROOT_DIR=/path/Demo/FFMpegDemo

export PATH=$PATH::$ROOT_DIR/deps:$ROOT_DIR/deps/bin:$ROOT_DIR/deps/include:$ROOT_DIR/deps/lib

make -j4

echo "FFMpeg Make done."

make install

echo "FFMpeg Install done."
```

4. FFMpegDemo工程的目标target的Target Dependencies中加上 **3** 中将FFMpeg工程的target(即 **FFMpeg_Building** )即可，**顺序位于Core构建之前(保证libavformat等源码的修改所编译出的.a为更改的最新)**

### 三、python脚本生成依赖调试

**于generateDep.py的目录下执行generateDep.py脚本，然后再重新cmake下主工程即可**

如：

```
# 即可在depsSource/ffmpeg-5.1.2中生成CMakeLists.txt
./generateDep.py depsSource/ffmpeg-5.1.2
```

> builddeps.py中**depsSourceFlag**设置为False，防止引过多源码
>  
> **步骤三为步骤二的自动化脚本处理**



