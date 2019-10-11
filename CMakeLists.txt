#This project tries to follow Modern CMAKE practices explained in 
#https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1

cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)
project(Noctis)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(dependencies)
add_subdirectory(source)

if(WIN32)
add_executable(${PROJECT_NAME} WIN32 source/winmain.cpp)
else(LINUX)
add_executable(${PROJECT_NAME} source/winmain.cpp)
endif(WIN32)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(LIBS assimp IMGUI)

#add directXTK if we are on Windows
if(WIN32)
	set(LIBS ${LIBS} directxtk)
endif(WIN32)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS} Math Engine Renderer ExampleApps)

# Add .lib files
target_link_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/dependencies/Assimp/lib
)
target_include_directories( ${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}/dependencies/Assimp/include
	${CMAKE_SOURCE_DIR}/dependencies/STBImage/include
	${CMAKE_SOURCE_DIR}/dependencies/DirectXTK/Inc
	${CMAKE_SOURCE_DIR}/dependencies/imgui
	${CMAKE_SOURCE_DIR}/dependencies
	${CMAKE_SOURCE_DIR}/source/Engine
	${CMAKE_SOURCE_DIR}/source/Math
	${CMAKE_SOURCE_DIR}/source/Renderer
	${CMAKE_SOURCE_DIR}/source/ExampleApps

)

# Copy dlls
if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/dlls"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()