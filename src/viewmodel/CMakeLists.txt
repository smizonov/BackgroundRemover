cmake_minimum_required(VERSION 3.14)

set(MY_MODULE_SOURCES
main.h
main.cpp
)

add_library(viewmodel STATIC ${MY_MODULE_SOURCES})
target_include_directories(viewmodel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(viewmodel ${OpenCV_LIBS})

