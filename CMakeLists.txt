cmake_minimum_required(VERSION 3.5)

project(HTTP-QT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)

link_directories(D:/cpp/dependences/mingw-lib/)

set(TS_FILES HTTP-QT_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
        about.h about.cpp about.ui
        ./dependences/extern_lib/httplib.h
        ./dependences/extern_lib/json.hpp
        ./dependences/HTMLFliter.h
        ./dependences/sizeTextHandler.h
        ./dependences/sizeTextHandler.cpp
        connect.h connect.cpp
        ip_controlpanel.h ip_controlpanel.cpp ip_controlpanel.ui
        propertieswidget.h propertieswidget.cpp propertieswidget.ui
        DelegateProgressBar.h
        FileList.h FileList.cpp

        ${TS_FILES}
)

set(QRC_SOURCE_FILES ./resource/svg/ArrowPack.qrc)
qt6_add_resources(QRC_FILES ${QRC_SOURCE_FILES})


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HTTP-QT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${QRC_FILES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET HTTP-QT APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(HTTP-QT SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(HTTP-QT
            ${PROJECT_SOURCES}
            ${QRC_FILES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

#httplib build required
target_link_libraries(HTTP-QT PRIVATE Qt${QT_VERSION_MAJOR}::Widgets libws2_32.a)

#QtConcurrent build required
target_link_libraries(HTTP-QT PRIVATE Qt6::Concurrent)

set_target_properties(HTTP-QT PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE FALSE
    WIN32_EXECUTABLE TRUE
)

target_precompile_headers(HTTP-QT PRIVATE precompiled_header.h)

install(TARGETS HTTP-QT
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HTTP-QT)
endif()
