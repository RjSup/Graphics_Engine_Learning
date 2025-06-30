cmake_minimum_required(VERSION 3.10)
project(Graphics_Engine)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

# Platform-specific configuration
if (WIN32)
    include_directories(
            "C:/gfx/glfw-3.4.bin.WIN64/include"
            "C:/gfx/glew-2.2.0/include/"
            "${PROJECT_SOURCE_DIR}/include"
            "${PROJECT_SOURCE_DIR}/res"
            "${PROJECT_SOURCE_DIR}/res/shaders"
    )

    link_directories(
            "C:/gfx/glfw-3.4.bin.WIN64/lib-vc2022"
            "C:/gfx/glew-2.2.0/lib/Release/x64"
    )

    set(PLATFORM_LIBS glfw3 glew32 opengl32)

elseif(APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    pkg_search_module(GLEW REQUIRED glew)

    include_directories(
            ${GLFW_INCLUDE_DIRS}
            ${GLEW_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/res
            ${PROJECT_SOURCE_DIR}/res/shaders
    )

    link_directories(
            ${GLFW_LIBRARY_DIRS}
            ${GLEW_LIBRARY_DIRS}
    )

    set(PLATFORM_LIBS ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} "-framework OpenGL")

elseif(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    pkg_search_module(GLEW REQUIRED glew)

    include_directories(
            ${GLFW_INCLUDE_DIRS}
            ${GLEW_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/res
            ${PROJECT_SOURCE_DIR}/res/shaders
    )

    link_directories(
            ${GLFW_LIBRARY_DIRS}
            ${GLEW_LIBRARY_DIRS}
    )

    set(PLATFORM_LIBS ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} GL X11 pthread dl)
endif()

add_executable(Graphics_Engine src/main.cpp)

target_link_libraries(Graphics_Engine ${PLATFORM_LIBS})
