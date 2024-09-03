cmake_minimum_required(VERSION 3.3)
project(Stun)

add_subdirectory(lib/glfw)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "src/*.h")
file(GLOB_RECURSE SHADER_FILES "res/shaders/*.shader")

source_group("Source Files" FILES ${SRC_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Shaders" FILES ${SHADER_FILES})

add_executable(Stun 
	${SRC_FILES}
	${HEADER_FILES}
	${SHADER_FILES}
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


target_include_directories(Stun 
	PRIVATE lib/glfw/include
	PRIVATE lib/glew/include
)