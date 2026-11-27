# ArchRenderer

ArchRenderer is a real-time 3D rendering application built with C++, OpenGL 4.1, GLFW, GLM, Assimp, and Dear ImGui. It loads and renders 3D models with directional lighting, free-fly camera controls, a cubemap skybox, and an integrated native UI inspector.

## Features

- Load and switch between 3D model formats supported by Assimp (`.obj`, `.glb`, `.gltf`, `.fbx`, `.dae`) directly from the UI.
- Real-time native Dear ImGui inspector for model transforms, lighting color/intensity, sun position, camera FOV, and wireframe/skybox toggles.
- Free-fly camera navigation with mouse look and keyboard movement, plus `Tab` cursor toggling for seamless UI interaction.
- Diffuse and specular lighting driven by a dynamic sun light source.
- Cubemap skybox rendering.

## Requirements

- CMake 3.20 or newer
- A C++17 compiler
- OpenGL 4.1+
- GLFW 3
- GLM
- Assimp

*(Dear ImGui and GLAD are vendored directly inside `engine/` and compiled automatically with the project—no external UI or Node.js dependencies are required.)*

The CMake configuration finds GLFW, GLM, and Assimp on your system. Install their development packages using the package manager for your operating system before building:

```sh
brew install cmake glfw glm assimp
```

## Build and Run

Run these commands from the repository root:

```sh
cmake -S engine -B build
cmake --build build --target ArchRender
```

Run the renderer from the repository root so its relative model, shader, and texture paths resolve correctly:

```sh
./build/ArchRender
```

## Controls

| Input | Action |
| --- | --- |
| Tab | Toggle between Fly Camera mode (cursor locked) and UI Inspector mode (cursor visible) |
| Mouse | Look around (in Fly Camera mode) |
| Scroll wheel | Change camera field of view |
| W / S | Move forward / backward |
| A / D | Move left / right |
| Q / E | Move up / down |
| Left Shift | Move forward faster |
| F | Toggle model rotation |
| I / J / K / L | Move the model |
| Space | Reset the model transform |
| Escape | Close the renderer |

## Project Layout

```text
engine/
├── CMakeLists.txt    # Build configuration for ArchRender and vendored ImGui
├── imgui/            # Dear ImGui core and GLFW/OpenGL3 backends
├── include/          # Camera, Shader, Model, Mesh, and UIManager headers
├── src/              # Main render loop and ImGui frontend implementation
├── shaders/          # GLSL vertex and fragment shaders
└── glad/             # OpenGL 4.1 Core function loader
models/               # Sample 3D models
textures/             # Cubemap and model textures
```

The cubemap face paths are configured in `engine/src/main.cpp`. Cubemaps require six square images with matching dimensions, in the order expected by the renderer: right, left, top, bottom, front, and back.
