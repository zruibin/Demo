
function(PROTOBUF_GENERATE_CPP SRCS HDRS ROOT_DIR)
    # 这里声明了三个参数, 传参有四个, ARGN 表示多出来的那些
    if(NOT ARGN)
        message(SEND_ERROR "Error: PROTOBUF_GENERATE_CPP() called without any proto files")
        return()
    endif()

    set(${SRCS})
    set(${HDRS})

    set(PROTOBUF_PROTOC_EXECUTABLE ${PROJECT_SOURCE_DIR}/deps/bin/protoc)
    set(PROTOBUF_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/include)

    foreach(FIL ${ARGN})
        set(ABS_FIL ${ROOT_DIR}/${FIL})
        get_filename_component(FIL_WE ${FIL} NAME_WE)
        get_filename_component(FIL_DIR ${ABS_FIL} PATH)
        file(RELATIVE_PATH REL_DIR ${ROOT_DIR} ${FIL_DIR})

        list(APPEND ${SRCS} "${PB_OUTPUT_DIR}/${FIL_WE}.pb.cc")
        list(APPEND ${HDRS} "${PB_OUTPUT_DIR}/${FIL_WE}.pb.h")

        message("---------------------------------------------------------------------")
        
        message("ABS_FIL: ${ABS_FIL}")
        message("PB_OUTPUT_DIR: ${PB_OUTPUT_DIR}")
        message("ROOT_DIR: ${ROOT_DIR}")
        message("PROTOBUF_INCLUDE_DIR: ${PROTOBUF_INCLUDE_DIR}")

        set(PROCESS_SHELL "${PROTOBUF_PROTOC_EXECUTABLE} --cpp_out=${PB_OUTPUT_DIR} ${ABS_FIL} -I ${ROOT_DIR} -I ${PROTOBUF_INCLUDE_DIR}")
        message("execute_process: ${PROCESS_SHELL}")
        string(REPLACE " " ";" PROCESS_SHELL_CMD ${PROCESS_SHELL})
        execute_process(COMMAND ${PROCESS_SHELL_CMD}
            OUTPUT_VARIABLE shell_stdout
            ERROR_VARIABLE shell_stderr
            RESULT_VARIABLE shell_result
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
        message("execute_process result -> shell_stdout:${shell_stdout}, shell_stderr:${shell_stderr}, shell_result:${shell_result}")

        message("---------------------------------------------------------------------")
    endforeach()

    set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
    set(${HDRS} ${${HDRS}} PARENT_SCOPE)

    set(cmake_string [[
message(STATUS "proto pb")

set(proto_name proto)

file(GLOB_RECURSE proto_all_file "${libs_dir}/proto/*")
sourceGroup("" ${proto_all_file})
    ]])
    file(WRITE "${PB_OUTPUT_DIR}/proto/CMakeLists.txt" "${cmake_string}")

endfunction()
