cmake_minimum_required(VERSION 3.14.3)
project(libstb)

set(STB_SOURCES 
    ${CMAKE_SOURCE_DIR}/dependencies/STBImage/stb_image.h
)

add_library(STB_IMAGE INTERFACE IMPORTED GLOBAL)

target_include_directories(STB_IMAGE INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(STB_IMAGE INTERFACE ${STB_SOURCES})

add_custom_target(stbimage SOURCES ${STB_SOURCES})

