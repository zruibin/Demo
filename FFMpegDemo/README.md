
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
              DW_AT_comp_dir    ("/path/FFMpegDemo/depsSource/ffmpeg-5.1.2/buildGen")
              DW_AT_low_pc      (0x0000000000000000)
              DW_AT_high_pc     (0x00000000000008d8)
```

## ffmpeg源码debug

1. configure时加上--extra-cflags=-g --extra-cflags=\"-fno-stack-check\" --enable-debug --disable-optimizations --disable-stripping

2. 将源码文件进xocde中，单步进入ffmpeg库调试

> ffmpeg源码中断点失效问题尚未解决







