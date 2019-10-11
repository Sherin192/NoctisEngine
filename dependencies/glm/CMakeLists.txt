cmake_minimum_required(VERSION 3.14.3)
project(libglm)

set(GLM_SOURCES 
    ${CMAKE_SOURCE_DIR}/dependencies/glm/glm.hpp
)

add_library(libglm INTERFACE IMPORTED GLOBAL)

target_include_directories(libglm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(libglm INTERFACE ${GLM_SOURCES})

add_custom_target(glm SOURCES ${GLM_SOURCES})
