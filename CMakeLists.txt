cmake_minimum_required(VERSION 3.10)
project(Graphics_Engine)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
include_directories("C:/gfx/glfw-3.4/glfw-3.4.bin.WIN64/include")
link_directories("C:/gfx/glfw-3.4/glfw-3.4.bin.WIN64/lib-mingw-w64")

set(SOURCE_FILES src/main.cpp)
add_executable(Graphics_Engine ${SOURCE_FILES})
target_link_libraries(Graphics_Engine ${OPENGL_LIBRARIES} glfw3 glu32 opengl32)