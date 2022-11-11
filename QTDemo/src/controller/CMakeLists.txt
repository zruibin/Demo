
message("Controller Module.")

set(controller_name controller)

file (GLOB_RECURSE controller_all_file "${src_dir}/controller/*")
sourceGroup("/src" ${controller_all_file})

if(NOT USE_DEPENDS_MODEL)
    add_library(${controller_name} ${controller_all_file})
endif()
