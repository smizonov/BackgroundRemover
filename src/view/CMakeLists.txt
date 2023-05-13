cmake_minimum_required(VERSION 3.14)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(MY_MODULE_SOURCES
mainwindow.qml
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BackgroundRemover
        MANUAL_FINALIZATION
        ${MY_MODULE_SOURCES}
    )
endif()

set_target_properties(view PROPERTIES
    AUTOMOC ON
    AUTORCC ON
)

add_library(view SHARED ${MY_MODULE_SOURCES})
target_include_directories(view PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

