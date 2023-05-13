cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

PROJECT(QTDemo)

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type was ${CMAKE_BUILD_TYPE}")

# 生成compile_commands.json,用于在VSCode等编辑器/IDE中给C/C++代码做函数定义跳转支持
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# 检查c++编译器标志，设置c++11支持变量
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

# message("List of compile features: ${CMAKE_CXX_COMPILE_FEATURES}")
# 设置c++的编译选项
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
if(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++17 support.")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler.")
endif()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")  #设置c的编译选项


if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message(WARNING "Debug Model.")
    add_definitions("-Wall")
    add_definitions("-fPIC")
    add_definitions("-Wl,-lm")
    add_definitions("-Wno-dev")  
    list(APPEND flags "-fPIC" "-Wall")
    if(NOT WIN32)
        list(APPEND flags "-Wextra" "-Wpedantic")
    endif()
endif()


message(STATUS "Is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")
if(CMAKE_CXX_COMPILER_LOADED)
    message(STATUS "The C++ compiler ID is: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "The C++ compiler version is: ${CMAKE_CXX_COMPILER_VERSION}")
endif()
message(STATUS "Is the C compiler loaded? ${CMAKE_C_COMPILER_LOADED}")
if(CMAKE_C_COMPILER_LOADED)
    message(STATUS "The C compiler ID is: ${CMAKE_C_COMPILER_ID}")
    message(STATUS "The C compiler version is: ${CMAKE_C_COMPILER_VERSION}")
endif()
message(STATUS "C flags, Debug configuration: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C flags, Release configuration: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "C flags, Release configuration with Debug info: ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "C flags, minimal Release configuration: ${CMAKE_C_FLAGS_MINSIZEREL}")
message(STATUS "C++ flags, Debug configuration: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags, Release configuration: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "C++ flags, Release configuration with Debug info: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "C++ flags, minimal Release configuration: ${CMAKE_CXX_FLAGS_MINSIZEREL}")


set(src_dir "${PROJECT_SOURCE_DIR}/src")
set(libs_dir "${PROJECT_SOURCE_DIR}/libs")

include_directories(
    "${src_dir}"
    "${libs_dir}"
)

# 相当于g++命令的-L选项的作用
link_directories(
    "/usr/local"
    "${src_dir}"
    "${libs_dir}"
)

# 添加依赖
if(EXISTS "${PROJECT_SOURCE_DIR}/deps.cmake")
    include("deps.cmake")
endif()

get_property(include_dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(WARNING ">>>include_dirs=${include_dirs}")
get_property(link_dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
message(WARNING ">>>link_dirs=${link_dirs}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(EXISTS "${PROJECT_SOURCE_DIR}/deps/") 
    set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/deps/")
    message(WARNING "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
endif() 

#在这里添加要使用的模块，不要忘记了下面链接时候也要添加相应模块
set(QtModule
    Widgets
    Core5Compat
    # LinguistTools  #翻译模块不用链接
    Network
    Multimedia
    MultimediaWidgets
    OpenGL
    OpenGLWidgets
)

find_package(QT NAMES Qt6 Qt5 COMPONENTS ${QtModule} REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QtModule} REQUIRED)

#在这里连接要链接的Qt模块
set(QtModuleLibs
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core5Compat
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
)


link_libraries(pthread dl)


function(deleteFiles)
    foreach(FILE IN LISTS ARGN) 
        if ("${FILE}" MATCHES ".DS_Store")
            message("删除文件: ${FILE}")
            file(REMOVE ${FILE})
        endif()
    endforeach()
endfunction()
file(GLOB_RECURSE delete_seerch_all_file "*")
deleteFiles(${delete_seerch_all_file})


# 按文件层次结构显示
function(sourceGroup prefix)
    message("prefix: ${prefix}")
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

# 开关，是否要源码依赖方式
set(USE_DEPENDS_MODEL TRUE)

# libs
#include("${libs_dir}/nlohmann/CMakeLists.txt")
# modules
include("${src_dir}/multimedia/CMakeLists.txt")
include("${src_dir}/controller/CMakeLists.txt")
include("${src_dir}/foundation/CMakeLists.txt")


if(USE_DEPENDS_MODEL)

set(libs_list 
)

set(modules_list 
)

set(DEPENDS_LIBS
    ${multimedia_all_file}
    ${controller_all_file}
    ${foundation_all_file}
)

else()

set(libs_list 
)

set(modules_list
    multimedia
    controller
    foundation
)

set(DEPENDS_LIBS 
)

endif()



set(all_list ${libs_list} ${modules_list})
message("all_list: ${all_list}")

set(HEADERS
    src/app.h
    src/window.h
)

set(SOURCES
    src/main.cc
    src/app.cc
    src/window.cc
)


file (GLOB_RECURSE ASSETS "assets/*")
sourceGroup("" ${ASSETS})

source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES})


if(WIN32)
    message(WARNING "Now is windows")
elseif(APPLE)
    message(WARNING "Now is Apple systens.")
    # Check whether we are running under Rosetta on arm64 hardware.
    execute_process(COMMAND sysctl -q hw.optional.arm64
        OUTPUT_VARIABLE _sysctl_stdout
        ERROR_VARIABLE _sysctl_stderr
        RESULT_VARIABLE _sysctl_result
    )
    if(_sysctl_result EQUAL 0 AND _sysctl_stdout MATCHES "hw.optional.arm64: 1")
        set(CMAKE_HOST_SYSTEM_PROCESSOR "arm64")
    endif()

    if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    else()
        set(CMAKE_OSX_ARCHITECTURES arm64)
    endif()
    message(STATUS "OSX Architectures: ${CMAKE_OSX_ARCHITECTURES}")
    message(WARNING "Host System Processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
elseif(UNIX)
    message(WARNING "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")
endif()

set(RESOURCES
    builddeps.py
    deps.json
    README.md
    LICENSE
)
if(EXISTS "${PROJECT_SOURCE_DIR}/deps.cmake")
    list(APPEND RESOURCES deps.cmake)
endif()
if(EXISTS "${PROJECT_SOURCE_DIR}/depsSource.lock")
    list(APPEND RESOURCES depsSource.lock)
endif()
if(EXISTS "${PROJECT_SOURCE_DIR}/depsSource.cmake")
    list(APPEND RESOURCES depsSource.cmake)
endif()
source_group(Resources FILES ${RESOURCES})


set(TARGET ${PROJECT_NAME})


#set(TARGET_STATIC_LIB "${PROJECT_NAME}_static")
#add_library(${TARGET_STATIC_LIB} STATIC ${DEPENDS_LIBS} ${HEADERS} ${SOURCES} ${all_list})
#set(TARGET_SHARED_LIB "${PROJECT_NAME}_shared")
#add_library(${TARGET_SHARED_LIB} SHARED ${DEPENDS_LIBS} ${HEADERS} ${SOURCES} ${all_list})

qt_add_executable(${TARGET} MANUAL_FINALIZATION
    ${DEPENDS_LIBS}
    ${HEADERS}
    ${SOURCES}
    ${ASSETS}
    ${RESOURCES}
)

set_target_properties(${TARGET} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

if(APPLE AND NOT IOS)
    set_target_properties(${TARGET} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/plist/macos/Info.plist.in"
    )
elseif(IOS)
    set_target_properties(${TARGET} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/plist/ios/Info.plist.in"
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        #MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION}
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources/assets"
)

target_compile_options(${TARGET} PRIVATE ${flags})
target_link_libraries(${TARGET} PRIVATE ${all_list} ${deps_list} ${QtModuleLibs})

qt_finalize_executable(${TARGET})


