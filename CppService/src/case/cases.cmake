
set(CASES_LIB "Case")

# ------------------------------------------------------------------------------

set(CASES_ROOT "src/case")
set(CASES_ROOT_PATH "${CMAKE_SOURCE_DIR}/${CASES_ROOT}")

set(BASE_PATH "${CASES_ROOT_PATH}/base")
set(CASES_PATH "${CASES_ROOT_PATH}/components")
set(SHELL_PATH "${CASES_ROOT_PATH}/shell")
set(CASE_INCLUDE_DIR "include")
set(CASE_SOURCE_DIR "source")

list(APPEND CASE_DIRECTORIES ${BASE_PATH})
list(APPEND EXPORT_CASE_HEADS_DIRECTORIES ${BASE_PATH})
list(APPEND CASE_DIRECTORIES ${CASES_PATH})
list(APPEND CASE_DIRECTORIES ${SHELL_PATH})


SUBDIRLIST(CASE_DIRS ${CASES_PATH})
foreach(subdir ${CASE_DIRS}) 
message("${subdir}: ${CASES_PATH}/${subdir}/${CASE_INCLUDE_DIR}")
message("${subdir}: ${CASES_PATH}/${subdir}/${CASE_SOURCE_DIR}")
list(APPEND CASE_DIRECTORIES ${CASES_PATH}/${subdir}/${CASE_INCLUDE_DIR})
list(APPEND CASE_DIRECTORIES ${CASES_PATH}/${subdir}/${CASE_SOURCE_DIR})
list(APPEND EXPORT_CASE_HEADS_DIRECTORIES ${CASES_PATH}/${subdir}/${CASE_INCLUDE_DIR})
endforeach()

SUBDIRLIST(SHELL_DIRS ${SHELL_PATH})
foreach(subdir ${SHELL_DIRS}) 
message("${subdir}: ${SHELL_PATH}/${subdir}")
list(APPEND CASE_DIRECTORIES ${SHELL_PATH}/${subdir})
endforeach()


foreach(directory IN LISTS CASE_DIRECTORIES)
    list(APPEND CASES_INTERNAL_HEADER_DIRS ${directory})

    file(GLOB_RECURSE files "${directory}/*.h")
    list(APPEND CASES_HEADERS "${files}")
    file(GLOB_RECURSE files "${directory}/*.hpp")
    list(APPEND CASES_HEADERS "${files}")
    
    file(GLOB_RECURSE files "${directory}/*.c")
    list(APPEND CASES_SOURCES "${files}")
    file(GLOB_RECURSE files "${directory}/*.cc")
    list(APPEND CASES_SOURCES "${files}")
    file(GLOB_RECURSE files "${directory}/*.cpp")
    list(APPEND CASES_SOURCES "${files}")
endforeach()

list(APPEND CASES_HEADERS "${CASES_ROOT_PATH}/CaseEngine.h")
list(APPEND CASES_SOURCES "${CASES_ROOT_PATH}/CaseEngine.cc")


list(APPEND EXPORT_CASE_HEADS_DIRECTORIES ${CASES_ROOT_PATH}/base)
foreach(directory IN LISTS EXPORT_CASE_HEADS_DIRECTORIES)
    file(GLOB_RECURSE files "${directory}/*.h")
    list(APPEND EXPORT_HEADS "${files}")
    file(GLOB_RECURSE files "${directory}/*.hpp")
    list(APPEND EXPORT_HEADS "${files}")

    list(APPEND EXPORT_HEADS_DIRECTORIES "${directory}")
endforeach()
list(APPEND EXPORT_HEADS "${CASES_ROOT_PATH}/CaseEngine.h")
list(APPEND EXPORT_HEADS_DIRECTORIES ${CASES_ROOT_PATH})

list(APPEND CASES_RESOURCES "${CASES_ROOT_PATH}/cases.cmake")
list(APPEND CASES_RESOURCES "${CASES_ROOT_PATH}/cases.py")
list(APPEND CASES_RESOURCES "${CASES_ROOT_PATH}/cases.lua")

sourceGroup(${CASES_ROOT}
    ${CASES_HEADERS} 
    ${CASES_SOURCES} 
    ${CASES_RESOURCES}
)

add_library(${CASES_LIB} STATIC 
    ${CASES_HEADERS} 
    ${CASES_SOURCES} 
    ${CASES_RESOURCES}
)

# 强制按照C++语言编译
set_target_properties(${CASES_LIB} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${CASES_LIB} PRIVATE 
    ${EXPORT_HEADS_DIRECTORIES} 
    ${CASES_INTERNAL_HEADER_DIRS}
)

list(APPEND EXPORT_LIBS ${CASES_LIB})
# 相当于g++选项中的-I参数的作用
include_directories(${EXPORT_HEADS_DIRECTORIES})


# message(STATUS "CASES_HEADERS: ${CASES_HEADERS}")
# message(STATUS "CASES_SOURCES: ${CASES_SOURCES}")
# message(STATUS "CASES_RESOURCES: ${CASES_RESOURCES}")
# message(STATUS "CASES_INTERNAL_HEADER_DIRS: ${CASES_INTERNAL_HEADER_DIRS}")

# message(STATUS "EXPORT_HEADS_DIRECTORIES: ${EXPORT_HEADS_DIRECTORIES}")
# message(STATUS "EXPORT_HEADS: ${EXPORT_HEADS}")
# message(STATUS "EXPORT_LIBS: ${EXPORT_LIBS}")

