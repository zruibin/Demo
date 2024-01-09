cmake_minimum_required (VERSION 2.8)

PROJECT(main)

# SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_BUILD_TYPE "Release")


# 检查c++编译器标志，设置c++11支持变量
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

# 使用变量设置编译标志
# if(COMPILER_SUPPORTS_CXX11)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# elseif(COMPILER_SUPPORTS_CXX0X)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
# else()
# message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
# endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")  #设置c++的编译选项
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")  #设置c的编译选项

ADD_DEFINITIONS("-Wall")
ADD_DEFINITIONS("-fPIC")
# ADD_DEFINITIONS("-Wl,-lm")
# ADD_DEFINITIONS("-Wno-dev")


# ADD_DEFINITIONS(
# -O3 -g -W -Wall
#  -Wunused-variable -Wunused-parameter -Wunused-function -Wunused
#  -Wno-deprecated -Woverloaded-virtual -Wwrite-strings
#  -D__WUR= -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DTIXML_USE_STL
# )

LINK_LIBRARIES(pthread dl)

include(../lib/lib.cmake)
include(./src.cmake)

# add_subdirectory(glad)

set(TARGET main)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

target_link_libraries(${TARGET} PRIVATE 
"-framework Cocoa" 
"-framework OpenGL" 
"-framework IOKit"
"-framework CoreFoundation" 
libglfw3.a
libglad.a
)

