cmake_minimum_required(VERSION 3.8)
project(libassimp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# ASSIMP
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${assimp_INCLUDE_DIR}")

file(GLOB_RECURSE assimp_INCLUDES 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assimp/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/assimp/*.hpp
)

add_custom_target(Assimp SOURCES ${assimp_INCLUDES})
set_target_properties(Assimp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include")


