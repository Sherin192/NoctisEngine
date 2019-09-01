cmake_minimum_required(VERSION 3.8)
project (ExampleApps)

add_library(ExampleApps
    ExampleDx11App.h
    ExampleDx11App.cpp
)

target_include_directories(ExampleApps PUBLIC
"${CMAKE_CURRENT_SOURCE_DIR}"
"${CMAKE_SOURCE_DIR}/dependencies"
"${CMAKE_SOURCE_DIR}/source"
"${CMAKE_SOURCE_DIR}/source/Engine"
"${CMAKE_SOURCE_DIR}/source/Renderer"
"${CMAKE_SOURCE_DIR}"
)