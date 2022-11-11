
message("Foundation Module.")

set(foundation_name foundation)

file (GLOB_RECURSE foundation_all_file "${src_dir}/foundation/*")
sourceGroup("/src" ${foundation_all_file})

if(NOT USE_DEPENDS_MODEL)
    add_library(${foundation_name} ${foundation_all_file})
endif()
