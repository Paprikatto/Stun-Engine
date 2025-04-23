cmake_minimum_required(VERSION 3.10)
project(StunEngine)
set(CXX_STANDARD C++17)
add_executable(StunEngine src/main.cpp
src/IndexBuffer.h src/IndexBuffer.cpp
src/Renderer.h src/Renderer.cpp
src/Shader.h src/Shader.cpp
src/Texture.h src/Texture.cpp
src/VertexArray.h src/VertexArray.cpp
src/VertexBuffer.h src/VertexBuffer.cpp
src/VertexBufferLayout.h src/VertexBufferLayout.cpp
src/stb/stb_image.h src/stb/stb_image.cpp
)
#Find Packages
# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# GLM
find_package(glm REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIRS}")

# GLFW
find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

find_package(GLEW REQUIRED)

add_library(imgui STATIC
    thirdparty/imgui/imgui.cpp
    thirdparty/imgui/imgui_draw.cpp
    thirdparty/imgui/imgui_tables.cpp
    thirdparty/imgui/imgui_widgets.cpp
    thirdparty/imgui/imgui_demo.cpp
    thirdparty/imgui/imgui_impl_glfw.cpp
    thirdparty/imgui/imgui_impl_opengl3.cpp
)
target_link_libraries(StunEngine
    glfw
    OpenGL::GL
    ${GLEW_LIBRARIES}
    imgui
)
include_directories(StunEngine PRIVATE "thirdparty")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#copy res folder to /build
add_custom_command(TARGET StunEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:StunEngine>/res
)
