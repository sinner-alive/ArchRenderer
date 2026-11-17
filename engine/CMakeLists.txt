
cmake_minimum_required(VERSION 3.20)

project(ArchRenderEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    enable_language(OBJCXX)
    set(CMAKE_OBJCXX_STANDARD 17)
    set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)
endif()

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

add_executable(ArchRender
    src/main.cpp
    glad/src/glad.c
)

if(APPLE)
    find_path(NODE_API_INCLUDE_DIR node_api.h
        PATHS /opt/homebrew/include/node /usr/local/include/node
        REQUIRED
    )
    find_library(COCOA_FRAMEWORK Cocoa REQUIRED)

    add_library(ArchRenderNative MODULE
        src/main.cpp
        src/native_addon.cpp
        src/macos_embedding.mm
        glad/src/glad.c
    )
    set_target_properties(ArchRenderNative PROPERTIES
        PREFIX ""
        SUFFIX ".node"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    target_compile_definitions(ArchRenderNative PRIVATE
        ARCHRENDER_NATIVE_ADDON
        NODE_GYP_MODULE_NAME=ArchRenderNative
    )
    target_include_directories(ArchRenderNative PRIVATE
        "${NODE_API_INCLUDE_DIR}"
        include
        glad/include
    )
    target_link_libraries(ArchRenderNative PRIVATE
        glfw
        OpenGL::GL
        glm::glm
        assimp::assimp
        "${COCOA_FRAMEWORK}"
    )
    target_link_options(ArchRenderNative PRIVATE "-undefined" "dynamic_lookup")
endif()

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