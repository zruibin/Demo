
set(HEADERS
)

set(SOURCES
    main.cc
)


source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES})

# if(NOT COMMON_DIR)
#     set(COMMON_DIR ${CMAKE_SOURCE_DIR}/../lib/common)
# endif()
# if(IS_DIRECTORY ${COMMON_DIR})
#     add_subdirectory(${COMMON_DIR} common.out)
# else()
#     message(FATAL_ERROR  "INVALID FOLDER 'COMMON_DIR'=${COMMON_DIR}" )
# endif()

# source_group(Common FILES ${Common})

include(../resources/res.cmake)