cmake_minimum_required(VERSION 3.8)
project(Renderer)

add_library(Renderer
    Dx11RenderDevice.h
    NoctisRenderDevice.cpp
    NoctisRenderDevice.h
    NoctisRenderDevice.cpp
    NoctisDx11RenderDevice.h
    NoctisDx11RenderDevice.cpp
    Dx11Utils.h
    InputLayout.h
    Dx11InputLayout.cpp
    Light.h
    Light.cpp
    Material.h
    Material.cpp
    Mesh.h
    Mesh.cpp
    Model.h
    Model.cpp
    Texture.h
    Texture.cpp
    NoctisShader.h
    NoctisShader.cpp
    Dx11Shader.h
    GLShader.h
    Noctis3DTypes.h
    NoctisBuffer.h
    NoctisConstantBuffer.h
    ConstantBufferData.h
    ConstantBufferData.cpp
    NoctisPipelinePass.h
    Vertex.h
)

target_include_directories(Renderer PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/dependencies"
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/source/Engine"
    "${CMAKE_SOURCE_DIR}"
)
