cmake_minimum_required(VERSION 3.3)
project(Stun)

add_subdirectory(lib/glfw)

add_executable(Stun src/main.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

target_include_directories(Stun 
	PRIVATE lib/glfw/include
	PRIVATE lib/glew/include
)