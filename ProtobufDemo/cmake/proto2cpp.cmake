
function(PROTOBUF_GENERATE ROOT_DIR PROTO_DIR OUTPUT_DIR)
    # ARGN 表示多出来的那些
    if(NOT ARGN)
        message(SEND_ERROR "Error: PROTOBUF_GENERATE() called without any proto files")
        return()
    endif()
    message("---------------------------------------------------------------------")

    message("ROOT_DIR: ${ROOT_DIR}")
    message("PROTO_DIR: ${PROTO_DIR}")
    message("OUTPUT_DIR: ${OUTPUT_DIR}")

    set(PROTOBUF_PROTOC_EXECUTABLE ${PROJECT_SOURCE_DIR}/deps/bin/protoc)
    set(PROTOBUF_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/deps/include)

    foreach(FIL ${ARGN})
        message("FIL: ${FIL}")
        set(PROCESS_SHELL "${PROTOBUF_PROTOC_EXECUTABLE} --cpp_out=${OUTPUT_DIR} ${FIL} -I ${ROOT_DIR} -I ${PROTOBUF_INCLUDE_DIR} -I=${PROTO_DIR}")
        message("execute_process: ${PROCESS_SHELL}")
        string(REPLACE " " ";" PROCESS_SHELL_CMD ${PROCESS_SHELL})

        execute_process(COMMAND ${PROCESS_SHELL_CMD}
            OUTPUT_VARIABLE shell_stdout
            ERROR_VARIABLE shell_stderr
            RESULT_VARIABLE shell_result
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
        message("execute_process result -> shell_stdout:${shell_stdout}, shell_stderr:${shell_stderr}, shell_result:${shell_result}")

    endforeach()

    set(cmake_string [=[
message(STATUS "proto pb")
    
set(proto_name proto)
    
file(GLOB_RECURSE proto_all_file "output_dir/*")
sourceGroup("" ${proto_all_file})
    
message( ${proto_all_file})
    ]=])
    string(REPLACE "output_dir" "${OUTPUT_DIR}" cmake_string_out ${cmake_string})
    message("cmake_string_out: ${cmake_string_out}")
    file(WRITE "${OUTPUT_DIR}/CMakeLists.txt" "${cmake_string_out}")

    message("---------------------------------------------------------------------")
endfunction()



