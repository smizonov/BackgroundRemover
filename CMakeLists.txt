cmake_minimum_required(VERSION 3.14)

project(BackgroundRemover VERSION 0.1 LANGUAGES CXX)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        AlgoInterface.h
        AlgoInterface.cpp
        RmBgMethods.h
        BgRemoverTask.cpp
        BgRemoverTask.h
)

add_subdirectory(src/model)

add_executable(BackgroundRemover
  ${PROJECT_SOURCES}
)

target_compile_definitions(BackgroundRemover
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(BackgroundRemover
  PRIVATE Qt5::Core Qt5::Quick)

set_target_properties(BackgroundRemover PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE model)

