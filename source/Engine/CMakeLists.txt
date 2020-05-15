cmake_minimum_required(VERSION 3.8)
project(Engine)

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
    Logger.h
    Logger.cpp
    NoctisMacros.h
    Light.h
    Light.cpp
    Model.h
    Model.cpp
)

target_include_directories(Engine PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/source/Math"
    "${CMAKE_SOURCE_DIR}/dependencies"
    "${CMAKE_SOURCE_DIR}/dependencies/spdlog/include"
    PRIVATE
    "${CMAKE_SOURCE_DIR}/dependencies/Assimp/include"
    
)