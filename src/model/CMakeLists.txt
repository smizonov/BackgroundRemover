cmake_minimum_required(VERSION 3.14)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(dlib
    GIT_REPOSITORY https://github.com/davisking/dlib.git
    GIT_TAG        v19.24
)
FetchContent_MakeAvailable(dlib)

set(SOURCES
    Ohlander.cpp
    Ohlander.h
    Dbscan.cpp
    Dbscan.h
    Bgsegm.cpp
    Bgsegm.h
Substruction.cpp
Substruction.h
BgRemover.h
BgRemover.cpp
BgRemoverSettings.h
SubsctructionSettings.h
MaskEditor.cpp
MaskEditor.h)

add_library(model STATIC
    ${SOURCES}
)

target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(model PRIVATE ${OpenCV_LIBS} Qt5::Core dlib::dlib)

#add_library(model STATIC ${MY_MODULE_SOURCES})
#target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(model ${OpenCV_LIBS})

