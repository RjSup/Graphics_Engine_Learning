cmake_minimum_required(VERSION 3.10)
project(Graphics_Engine)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

# Header paths
include_directories(
        "C:/gfx/glfw-3.4/glfw-3.4.bin.WIN64/include"
        "C:/gfx/glew-2.2.0/include"
)

# Linker search paths
link_directories(
        "C:/gfx/glfw-3.4/glfw-3.4.bin.WIN64/lib-mingw-w64"
        "C:/gfx/glew-2.2.0/lib"
)

add_executable(Graphics_Engine src/main.cpp)

# Libraries to link
target_link_libraries(Graphics_Engine
        glfw3
        glew32
        ${OPENGL_LIBRARIES}
        glu32
        opengl32
)
