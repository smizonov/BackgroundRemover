cmake_minimum_required(VERSION 3.14)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

install(FILES OpenCV DESTINATION ${CMAKE_BINARY_DIR})
#install(FILES ${OpenCV_INCLUDE_DIRS} DESTINATION ${CMAKE_BINARY_DIR})

set(SOURCES
    Ohlander.cpp
    Ohlander.h
    KMeans.cpp
    KMeans.h
    Bgsegm.cpp
    Bgsegm.h
Substruction.cpp
Substruction.h
BgRemover.h
BgRemover.cpp
BgRemoverSettings.h
SubstructionSettings.h
MaskEditor.cpp
MaskEditor.h
MlRemover.h
MlRemover.cpp
ModelWorker.h
ModelWorker.cpp)

add_library(model STATIC
    ${SOURCES}
)

target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(model PRIVATE ${OpenCV_LIBS} Qt5::Core)

#add_library(model STATIC ${MY_MODULE_SOURCES})
#target_include_directories(model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(model ${OpenCV_LIBS})

