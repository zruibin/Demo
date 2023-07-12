
set(COMPONENT_PATH "${CMAKE_SOURCE_DIR}/src")

list(APPEND DIRECTORIES ${COMPONENT_PATH}/base)
list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENT_PATH}/base)
list(APPEND DIRECTORIES ${COMPONENT_PATH}/shell)
# AudioDect
list(APPEND DIRECTORIES ${COMPONENT_PATH}/shell/AudioDectBuilder)
list(APPEND DIRECTORIES ${COMPONENT_PATH}/components/AudioDect/include)
list(APPEND DIRECTORIES ${COMPONENT_PATH}/components/AudioDect/source)
list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENT_PATH}/components/AudioDect/include)
# Service
list(APPEND DIRECTORIES ${COMPONENT_PATH}/shell/ServiceBuilder)
list(APPEND DIRECTORIES ${COMPONENT_PATH}/components/Service/include)
list(APPEND DIRECTORIES ${COMPONENT_PATH}/components/Service/source)
list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENT_PATH}/components/Service/include)

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

list(APPEND HEADERS "${COMPONENT_PATH}/Engine.h")
list(APPEND SOURCES "${COMPONENT_PATH}/Engine.cc")


list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENT_PATH}/base)
list(APPEND EXPORT_HEADS_DIRECTORIES ${COMPONENT_PATH}/components/service/include)
foreach(directory IN LISTS EXPORT_HEADS_DIRECTORIES)
    file(GLOB_RECURSE files "${directory}/*.h")
    list(APPEND EXPORT_HEADS "${files}")
    file(GLOB_RECURSE files "${directory}/*.hpp")
    list(APPEND EXPORT_HEADS "${files}")
endforeach()
list(APPEND EXPORT_HEADS "${COMPONENT_PATH}/Engine.h")

list(APPEND RESOURCES "${COMPONENT_PATH}/components.cmake")
list(APPEND RESOURCES "${COMPONENT_PATH}/components.py")


# message(STATUS "HEADERS: ${HEADERS}")
# message(STATUS "SOURCES: ${SOURCES}")
# message(STATUS "EXPORT_HEADS: ${EXPORT_HEADS}")
# message(STATUS "RESOURCES: ${RESOURCES}")
    