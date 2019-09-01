cmake_minimum_required(VERSION 3.8)
project(libimgui)

file(GLOB_RECURSE IMGUI_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h 
)
file(GLOB_RECURSE IMGUI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


add_library(IMGUI STATIC ${IMGUI_INCLUDES} ${IMGUI_SOURCES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(IMGUI PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)


