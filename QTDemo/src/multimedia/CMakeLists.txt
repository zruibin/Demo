
message("multimedia Module.")

set(multimedia_name multimedia)

file (GLOB_RECURSE multimedia_all_file "${src_dir}/multimedia/*")
sourceGroup("/src" ${multimedia_all_file})

if(NOT USE_DEPENDS_MODEL)
    add_library(${multimedia_name} ${multimedia_all_file})
endif()
