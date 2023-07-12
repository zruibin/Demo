cmake_minimum_required (VERSION 3.0)

# projectname is the same as the main-executable
project(CppService)
set(TARGET "CppService")
set(LIB_NAME "Service")


option(CMAKE_BUILD_TYPE "Build Type." Release)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(WARNING "Build Type: ${CMAKE_BUILD_TYPE}")

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
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99") 


if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message(STATUS "Debug Type.")
    add_definitions("-Wall")
    add_definitions("-fPIC")
    add_definitions("-Wl,-lm")
    add_definitions("-Wno-dev")
    if(NOT WIN32)
        add_definitions("-Wextra")
        add_definitions("-Wpedantic")
    endif()
endif()

#-------------------------------------------------------------------------------

function(deleteFiles)
    foreach(FILE IN LISTS ARGN) 
        if ("${FILE}" MATCHES ".DS_Store")
            message(WARNING "删除文件: ${FILE}")
            file(REMOVE ${FILE})
        endif()
    endforeach()
endfunction()
file(GLOB_RECURSE delete_seerch_all_file "*")
deleteFiles(${delete_seerch_all_file})

# default was /src
set(sourceGroupPrefixName "")

# 按文件层次结构显示
function(sourceGroup prefix)
    #message("prefix: ${prefix}")
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


#-------------------------------------------------------------------------------

if(WIN32)
    message(WARNING "Now is windows")
elseif(APPLE)
    message(STATUS "Now is Apple systens.")

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
        add_definitions("-D__x86_64__=1")
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    else()
        set(CMAKE_OSX_ARCHITECTURES arm64)
    endif()
    message(STATUS "OSX Architectures: ${CMAKE_OSX_ARCHITECTURES}")
    message(WARNING "Host System Processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
elseif(UNIX)
    message(WARNING "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")
endif()
message(WARNING "Operation System: ${CMAKE_SYSTEM}")

#-------------------------------------------------------------------------------

include("${CMAKE_SOURCE_DIR}/src/components.cmake")

# 相当于g++命令的-L选项的作用
link_directories(
    "/usr/local"
)

set(EXECUTE_SOURCES
    src/main.cc
)

source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES} ${EXECUTE_SOURCES})
source_group(Resources FILES ${RESOURCES})

#-------------------------------------------------------------------------------

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/Bin)


add_executable(${TARGET} ${EXECUTE_SOURCES} ${RESOURCES})

set(TARGET_LIB "${LIB_NAME}")
add_library(${TARGET_LIB} STATIC ${HEADERS} ${SOURCES})
target_link_libraries(${TARGET} PRIVATE ${TARGET_LIB})




#-------------------------------------------------------------------------------

message(STATUS "origin installed directory: ${CMAKE_INSTALL_PREFIX}")
if(${CMAKE_INSTALL_PREFIX} MATCHES "/usr/local")
    set(install_directory "${PROJECT_SOURCE_DIR}/install")
    file(MAKE_DIRECTORY ${install_directory})
    set(CMAKE_INSTALL_PREFIX ${install_directory})
    message(STATUS "Create installed directory: ${CMAKE_INSTALL_PREFIX}")
endif()
message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")

install(
    TARGETS ${TARGET} ${TARGET_LIB}
    RUNTIME DESTINATION bin 
    LIBRARY DESTINATION lib
    BUNDLE DESTINATION bundle
)

install(FILES ${EXPORT_HEADS} DESTINATION include)






