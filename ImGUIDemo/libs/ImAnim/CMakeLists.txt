
message("ImAnim generate on https://github.com/cipperly/ImAnim.git")

set(ImAnim_name ImAnim)

file(GLOB_RECURSE ImAnim_all_file "${libs_dir}/ImAnim/*")
sourceGroup("" ${ImAnim_all_file})
