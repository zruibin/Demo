cmake_minimum_required(VERSION 3.15)

project(TestApp LANGUAGES CXX)
project(TestApp VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#在这里添加要使用的模块，不要忘记了下面链接时候也要添加相应模块
set(QtModule
    Widgets
    Core5Compat
    LinguistTools  #翻译模块不用链接
    #    Network
    )

find_package(QT NAMES Qt6 Qt5 COMPONENTS ${QtModule} REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QtModule} REQUIRED)


include_directories(${PROJECT_SOURCE_DIR}/include)


FILE(GLOB_RECURSE HEADERS   "include/*.h")
aux_source_directory(src SOURCES)
#FILE(GLOB_RECURSE SOURCES   "src/*.cpp src/*.cc src/*.h src/*.hh src/*.hpp src/*.ui")
FILE(GLOB_RECURSE FORMS     "ui/*.ui")
FILE(GLOB_RECURSE RESOURCES "qrc/*.qrc")
FILE(GLOB_RECURSE WINRC     "winrc/*.rc")

# message(STATUS ${FORMS})
# 自动UIC: set(CMAKE_AUTOUIC ON)
#手动进行UIC
if(${QT_VERSION_MAJOR} EQUAL 6)
    QT_WRAP_UI(SOURCES ${FORMS})
elseif(${QT_VERSION_MAJOR} EQUAL 5)
    QT5_WRAP_UI(SOURCES ${FORMS})
else()
    #qt5 以下版本
    QT4_WRAP_UI(SOURCES2 ${FORMS})
endif()
message(${SOURCES})

set(TS_FILES
    ts/TestApp_zh_CN.ts
    ts/TestApp_en.ts
    )

if(ANDROID)
    add_library(${PROJECT_NAME} SHARED
        ${HEADERS}
        ${SOURCES} 
        ${RESOURCES}
        ${WINRC}
        ${TS_FILES}
        )
else()

    add_executable(${PROJECT_NAME}
        ${HEADERS}
        ${SOURCES}
        ${RESOURCES}
        ${WINRC}
        ${TS_FILES}
        )
endif()

#在这里连接要链接的Qt模块
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core5Compat
#    Qt${QT_VERSION_MAJOR}::Network
)

#设置翻译文件

if(${QT_VERSION_MAJOR} EQUAL 6)
    qt6_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
elseif(${QT_VERSION_MAJOR} EQUAL 5)
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    #qt5 以下版本
endif()

#设置输出目录
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(platform x64)
ELSE()
    set(platform x86)
ENDIF()
 
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${platform}/${CMAKE_BUILD_TYPE})
message(STATUS ${EXECUTABLE_OUTPUT_PATH})
