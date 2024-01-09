message("这是lib.cmake!")
message(${CMAKE_CURRENT_SOURCE_DIR})
message(${CMAKE_SOURCE_DIR})

set(INCLUDEDIR "${CMAKE_SOURCE_DIR}/../lib")

# 相当于g++选项中的-I参数的作用
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}
    ${INCLUDEDIR}
)
# 相当于g++命令的-L选项的作用
LINK_DIRECTORIES(

)