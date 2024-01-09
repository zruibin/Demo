
message("glad generate on https://gen.glad.sh/")

set(glad_name glad)

file (GLOB_RECURSE glad_all_file "${libs_dir}/glad/*")
sourceGroup("" ${glad_all_file})
