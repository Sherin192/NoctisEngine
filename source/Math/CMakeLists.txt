cmake_minimum_required(VERSION 3.14.3)
project(Math)


add_library(Math
    NoctisVector.h
    NoctisMatrix.h
    NoctisMatrix.cpp
    NoctisMath.h
)

target_include_directories(Math PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/dependencies"
)