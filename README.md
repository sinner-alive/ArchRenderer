# ArchRenderer

ArchRenderer is a real-time 3D rendering project built with C++, OpenGL, GLFW, GLM, and Assimp. It loads and renders 3D models with lighting, camera controls, and a cubemap skybox. An Electron-based desktop interface is also included.

## Features

- Load and render 3D model formats supported by Assimp.
- Orbit the view with mouse look and move the camera with the keyboard.
- Apply diffuse and specular lighting.
- Render a cubemap skybox.
- Display a sun model in the scene.

## Requirements

- CMake 3.20 or newer
- A C++17 compiler
- OpenGL
- GLFW 3
- GLM
- Assimp
- Node.js and npm (only needed for the Electron interface)

The native engine's CMake configuration finds GLFW, GLM, and Assimp on your system. Install their development packages using the package manager for your operating system before building.

## Build the native renderer

Run these commands from the repository root:

```sh
cmake -S engine -B build
cmake --build build --target ArchRender
```

Run the renderer from the repository root so its relative model, shader, and texture paths resolve correctly:

```sh
./build/ArchRender
```

## Run the Electron interface

Build the native renderer first, then install the JavaScript dependencies and start Electron:

```sh
npm install
npm start
```

The Electron application launches `build/ArchRender` from the repository root.

## Controls

| Input | Action |
| --- | --- |
| Mouse | Look around |
| Scroll wheel | Change camera field of view |
| W / S | Move forward / backward |
| A / D | Move left / right |
| Q / E | Move up / down |
| Left Shift | Move forward faster |
| F | Toggle model rotation |
| I / J / K / L | Move the model |
| Space | Reset the model transform |
| Escape | Close the renderer |

## Project layout

```text
engine/       C++ renderer, headers, and GLSL shaders
frontend/     Electron application's HTML, CSS, and JavaScript
electron/     Electron main process and preload script
models/       Sample 3D models
textures/     Cubemap and model textures
```

The cubemap face paths are configured in `engine/src/main.cpp`. Cubemaps require six square images with matching dimensions, in the order expected by the renderer: right, left, top, bottom, front, and back.
