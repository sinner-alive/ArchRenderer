cmake_minimum_required(VERSION 3.20)

project(ArchRenderEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

add_library(imgui STATIC
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    imgui
    imgui/backends
    glad/include
)

target_link_libraries(imgui PUBLIC
    glfw
    OpenGL::GL
)

add_executable(ArchRender
    src/uimanager.cpp
    src/main.cpp
    glad/src/glad.c
)

target_include_directories(ArchRender PRIVATE
    include
    glad/include
)

target_link_libraries(ArchRender PRIVATE
    imgui
    glfw
    OpenGL::GL
    glm::glm
    assimp::assimp
)