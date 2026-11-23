
cmake_minimum_required(VERSION 3.20)

project(ArchRenderEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

add_executable(ArchRender
    src/main.cpp
    glad/src/glad.c
)

target_include_directories(ArchRender PRIVATE
    include
    glad/include
)
target_link_libraries(ArchRender PRIVATE
    glfw
    OpenGL::GL
    glm::glm
    assimp::assimp
)