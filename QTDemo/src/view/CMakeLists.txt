
if(BUILD_WITH_VIEW)

message("View Module.")

set(view_name view)

file (GLOB_RECURSE view_all_file "${src_dir}/view/*")
sourceGroup("/src" ${view_all_file})


add_definitions("-DVIEW_DISPLAY=1")


#在这里添加要使用的模块，不要忘记了下面链接时候也要添加相应模块
set(QtModule
    Widgets
    Core5Compat
    Multimedia
    MultimediaWidgets
    OpenGL
    OpenGLWidgets
)

find_package(QT NAMES Qt6 Qt5 COMPONENTS ${QtModule} REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QtModule} REQUIRED)


#在这里连接要链接的Qt模块
set(QtModuleLibs
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core5Compat
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
)

link_libraries(pthread dl)

endif()


