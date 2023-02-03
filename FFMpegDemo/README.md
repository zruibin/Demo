
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