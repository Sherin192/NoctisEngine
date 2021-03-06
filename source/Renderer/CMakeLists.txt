cmake_minimum_required(VERSION 3.8)
project(Renderer)

add_library(Renderer
    NoctisRenderDevice.cpp
    NoctisRenderDevice.h
    NoctisRenderDevice.cpp
    Dx11Utils.h
    InputLayout.h
    Dx11InputLayout.cpp
    Material.h
    Material.cpp
    Mesh.h
    Mesh.cpp
    NoctisTexture.h
    NoctisTexture.cpp
    Dx11Texture.h
    Dx11Texture.cpp
    NoctisSampler.h
    Dx11Sampler.h
    Dx11RenderDevice.h
    Dx11RenderDevice.cpp
    NoctisShader.h
    NoctisShader.cpp
    Dx11Shader.h
    GLShader.h
    Noctis3DTypes.h
    Dx11Buffer.h
    NoctisBuffers.h
    ConstantBufferData.h
    ConstantBufferData.cpp
    NoctisPipelinePass.h
    Vertex.h
    NoctisCubeMap.h
    NoctisCubeMap.cpp
    NoctisRenderable.h
)

target_include_directories(Renderer PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/dependencies"
	"${CMAKE_SOURCE_DIR}/dependencies/spdlog/include"
    "${CMAKE_SOURCE_DIR}/source"
    "${CMAKE_SOURCE_DIR}/source/Engine"
    "${CMAKE_SOURCE_DIR}/source/Math"
    "${CMAKE_SOURCE_DIR}"
)
