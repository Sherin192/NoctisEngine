cmake_minimum_required(VERSION 3.8)
project(Engine)

#add_subdirectory(Renderer)

add_library(Engine
    Window.h
    Window.cpp
    App.h
    Dx11App.h
    Dx11App.cpp
    Core_pch.h
    Clock.h
    Transform.h
    CameraFPS.h
    CameraFPS.cpp
    ShapeGenerator.h
    ShapeGenerator.cpp
    AssetImporter.h
    AssetImporter.cpp
    AssetManager.h
    AssetManager.cpp
)

target_include_directories(Engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/dependencies"
    PRIVATE
    "${CMAKE_SOURCE_DIR}/dependencies/Assimp/include"

)