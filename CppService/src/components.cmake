

MACRO(COMPONENTS_SUBDIRLIST result curdir)
FILE(GLOB children RELATIVE ${curdir} "${curdir}/*")
SET(dirlist "")
FOREACH(child ${children})
  IF(IS_DIRECTORY ${curdir}/${child})
    LIST(APPEND dirlist ${child})
  ENDIF()
ENDFOREACH()
SET(${result} ${dirlist})
ENDMACRO()

# ------------------------------------------------------------------------------

set(COMPONENTS_ROOT_PATH "${CMAKE_SOURCE_DIR}/src")

set(BASE_PATH "${COMPONENTS_ROOT_PATH}/base")
set(COMPONENTS_PATH "${COMPONENTS_ROOT_PATH}/components")
set(SHELL_PATH "${COMPONENTS_ROOT_PATH}/shell")
set(COMPONENT_INCLUDE_DIR "include")
set(COMPONENT_SOURCE_DIR "source")

list(APPEND DIRECTORIES ${BASE_PATH})
list(APPEND EXPORT_HEADS_DIRECTORIES ${BASE_PATH})
list(APPEND DIRECTORIES ${COMPONENTS_PATH})
list(APPEND DIRECTORIES ${SHELL_PATH})


COMPONENTS_SUBDIRLIST(COMPONENT_DIRS ${COMPONENTS_PATH})
foreach(subdir ${COMPONENT_DIRS}) 
message("${subdir}: ${COMPONENTS_PATH}/${subdir}/${COMPONENT_INCLUDE_DIR}")
message("${subdir}: ${COMPONENTS_PATH}/${subdir}/${COMPONENT_SOURCE_DIR}")
list(APPEND DIRECTORIES ${COMPONENTS_PATH}/${subdir}/${COMPONENT_INCLUDE_DIR})
list(APPEND DIRECTORIES ${COMPONENTS_PATH}/${subdir}/${COMPONENT_SOURCE_DIR})
list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENTS_PATH}/${subdir}/${COMPONENT_INCLUDE_DIR})
endforeach()

COMPONENTS_SUBDIRLIST(SHELL_DIRS ${SHELL_PATH})
foreach(subdir ${SHELL_DIRS}) 
message("${subdir}: ${SHELL_PATH}/${subdir}")
list(APPEND DIRECTORIES ${SHELL_PATH}/${subdir})
endforeach()


foreach(directory IN LISTS DIRECTORIES)
    # 相当于g++选项中的-I参数的作用
    include_directories(${directory})

    file(GLOB_RECURSE files "${directory}/*.h")
    list(APPEND HEADERS "${files}")
    file(GLOB_RECURSE files "${directory}/*.hpp")
    list(APPEND HEADERS "${files}")
    
    file(GLOB_RECURSE files "${directory}/*.c")
    list(APPEND SOURCES "${files}")
    file(GLOB_RECURSE files "${directory}/*.cc")
    list(APPEND SOURCES "${files}")
    file(GLOB_RECURSE files "${directory}/*.cpp")
    list(APPEND SOURCES "${files}")
endforeach()

list(APPEND HEADERS "${COMPONENTS_ROOT_PATH}/Engine.h")
list(APPEND SOURCES "${COMPONENTS_ROOT_PATH}/Engine.cc")


list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENTS_ROOT_PATH}/base)
foreach(directory IN LISTS EXPORT_HEADS_DIRECTORIES)
    file(GLOB_RECURSE files "${directory}/*.h")
    list(APPEND EXPORT_HEADS "${files}")
    file(GLOB_RECURSE files "${directory}/*.hpp")
    list(APPEND EXPORT_HEADS "${files}")
endforeach()
list(APPEND EXPORT_HEADS "${COMPONENTS_ROOT_PATH}/Engine.h")

list(APPEND RESOURCES "${COMPONENTS_ROOT_PATH}/components.cmake")
list(APPEND RESOURCES "${COMPONENTS_ROOT_PATH}/components.py")


# message(STATUS "HEADERS: ${HEADERS}")
# message(STATUS "SOURCES: ${SOURCES}")
# message(STATUS "EXPORT_HEADS: ${EXPORT_HEADS}")
# message(STATUS "RESOURCES: ${RESOURCES}")