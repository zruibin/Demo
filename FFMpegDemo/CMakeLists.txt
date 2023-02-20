cmake_minimum_required (VERSION 3.0)

# projectname is the same as the main-executable
project(FFMpegDemo)
set(TARGET ${PROJECT_NAME})
set(LIB_NAME "Core")


option(CMAKE_BUILD_TYPE "Build Type." Release)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
message(WARNING "Build Type: ${CMAKE_BUILD_TYPE}")


option(BUILD_WITH_LIBS "Build With Libs." ON)
message(WARNING "Build With Libs: ${BUILD_WITH_LIBS}")
option(BUILD_WITH_VIEW "Build With View." OFF)
message(WARNING "Build With View: ${BUILD_WITH_VIEW}")


# 生成compile_commands.json,用于在VSCode等编辑器/IDE中给C/C++代码做函数定义跳转支持
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


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

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftime-trace")
    message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()

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

add_definitions("-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\"")

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

    list(APPEND DEPS_LIBS "-framework Foundation")
    list(APPEND DEPS_LIBS "-framework OpenGL")
    list(APPEND DEPS_LIBS "-framework GLUT")
    list(APPEND DEPS_LIBS "-framework CoreServices")
    list(APPEND DEPS_LIBS "-framework Metal")
    list(APPEND DEPS_LIBS "-framework AVFoundation")
    list(APPEND DEPS_LIBS "-framework VideoToolbox")
    list(APPEND DEPS_LIBS "-framework AudioUnit")
    list(APPEND DEPS_LIBS "-framework AudioToolbox")
    list(APPEND DEPS_LIBS "-framework CoreAudio")
    list(APPEND DEPS_LIBS "-framework ApplicationServices")
    list(APPEND DEPS_LIBS "-framework Security")
    list(APPEND DEPS_LIBS "-framework CoreMedia")
    list(APPEND DEPS_LIBS "bz2")
    list(APPEND DEPS_LIBS "iconv")
    list(APPEND DEPS_LIBS "lzma")
elseif(UNIX)
    message(WARNING "Now is UNIX-like OS's. Including aPPLE os x  and CygWin")
endif()
message(WARNING "Operation System: ${CMAKE_SYSTEM}")

#-------------------------------------------------------------------------------

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

# modules
include("${src_dir}/view/CMakeLists.txt")
include("${src_dir}/log/CMakeLists.txt")
include("${src_dir}/platform/CMakeLists.txt")
include("${src_dir}/util/CMakeLists.txt")
include("${src_dir}/foundation/CMakeLists.txt")

# libs

set(MODULES
    ${log_all_file}
    ${platform_all_file}
    ${util_all_file}
    ${foundation_all_file}
)

set(DEPENDS_LIBS
    
)

list(APPEND ALL_SOURCES ${MODULES} ${DEPENDS_LIBS})

#-------------------------------------------------------------------------------

set(HEADERS
    
)

set(SOURCES
    
)

set(EXECUTE_SOURCES
    src/app.h
    src/app.cc
    src/main.cc
)

# 添加依赖
if(EXISTS "${PROJECT_SOURCE_DIR}/deps.cmake")
    message(STATUS "Include deps.cmake")
    include("deps.cmake")
endif()

source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES} ${EXECUTE_SOURCES})

#-------------------------------------------------------------------------------

# 相当于g++选项中的-I参数的作用
include_directories(

)

#-------------------------------------------------------------------------------

set(RESOURCES
    builddeps.py
    generateDep.py
    deps.json
    README.md
    LICENSE
)
if(EXISTS "${PROJECT_SOURCE_DIR}/deps.cmake")
    list(APPEND RESOURCES deps.cmake)
endif()
if(EXISTS "${PROJECT_SOURCE_DIR}/depsSource/depsSource.lock")
    list(APPEND RESOURCES depsSource/depsSource.lock)
endif()
if(EXISTS "${PROJECT_SOURCE_DIR}/depsSource.cmake")
    list(APPEND RESOURCES depsSource.cmake)
endif()
source_group(Resources FILES ${RESOURCES})

#-------------------------------------------------------------------------------

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/Bin)
message(STATUS "OUTPUT_DIR: ${OUTPUT_DIR}")
message(STATUS "ARCHIVE_OUTPUT_DIRECTORY: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

if(BUILD_WITH_LIBS)
    set(TARGET_LIB "${LIB_NAME}")
    add_library(${TARGET_LIB} STATIC ${ALL_SOURCES} ${HEADERS} ${SOURCES})
    #add_library(${TARGET_LIB} SHARED ${ALL_SOURCES} ${HEADERS} ${SOURCES})
else()
    list(APPEND EXECUTE_SOURCES ${ALL_SOURCES} ${HEADERS} ${SOURCES})
endif()


if(BUILD_WITH_VIEW)

file (GLOB_RECURSE ASSETS "assets/*")
sourceGroup("" ${ASSETS})

qt_add_executable(${TARGET} MANUAL_FINALIZATION
    ${view_all_file}
    ${EXECUTE_SOURCES}
    ${ASSETS}
    ${RESOURCES}
)

set_target_properties(${TARGET} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

set_target_properties(${TARGET} PROPERTIES
    AUTOMOC TRUE
    AUTOUIC TRUE
    AUTORCC TRUE
)

if(APPLE AND NOT IOS)
    set_target_properties(${TARGET} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/assets/plist/Info.plist.in"
    )
elseif(IOS)
    set_target_properties(${TARGET} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/assets/plist/Info.plist.in"
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        #MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION}
    )
endif()


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../Resources/assets"
)

target_link_libraries(${TARGET} PRIVATE
    ${QtModuleLibs}
    ${TARGET_LIB}
    ${DEPS_LIBS}
)

qt_finalize_executable(${TARGET})

else()

add_executable(${TARGET} ${EXECUTE_SOURCES} ${RESOURCES})
target_link_libraries(${TARGET} PRIVATE ${TARGET_LIB} ${DEPS_LIBS})

endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/depsSource.cmake")
    include("depsSource.cmake")
endif()
if(Deps_Source_Targets)
    add_dependencies(${TARGET} ${Deps_Source_Targets})
endif()

#-------------------------------------------------------------------------------

# 单元测试
option(ENABLE_UNIT_TESTS "Enable Unit Tests." OFF)
if(ENABLE_UNIT_TESTS)
    include("${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
endif()
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

set(output_head_files
    "${src_dir}/app.h"
)
install(FILES ${output_head_files} DESTINATION include)

install(DIRECTORY src/foundation
        DESTINATION include
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ)


set(output_libs_head_files_directory
    webrtc/include/
    webrtc/include_third_party/
    ${DEPS_INCLUDE_DIR}
)
install(
    DIRECTORY ${output_libs_head_files_directory}
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_EXECUTE GROUP_READ
)
install(
    DIRECTORY ${DEPS_LIB_DIR}
    DESTINATION lib
    PATTERN "cmake" EXCLUDE
    PATTERN "pkgconfig" EXCLUDE
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_EXECUTE GROUP_READ
)

#-------------------------------------------------------------------------------

if(CMAKE_GENERATOR MATCHES "Unix Makefiles")
    set(RUN_NAME "run")
    message(STATUS "CMAKE_GENERATOR: ${CMAKE_GENERATOR}, and generate target(${RUN_NAME})")
    add_custom_target(${RUN_NAME}
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Intermediate Run target: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
endif()


