# ArchRender

ArchRender is a desktop 3D architecture viewer built with Electron, C++, OpenGL, and a native engine layer for real-time rendering of architectural models. It combines a lightweight desktop shell with a custom OpenGL renderer for exploring imported scenes, model assets, and viewport-based composition workflows.

## Overview

This project is designed to provide a fast and extensible platform for viewing and interacting with 3D architectural content. The current implementation focuses on:

- Loading and rendering 3D models
- Displaying them in a dedicated viewport
- Supporting camera navigation and scene inspection
- Providing an Electron-based desktop interface for model import and editor-style controls

## Features

- Native C++ rendering engine powered by OpenGL
- Electron desktop application shell
- Model import support for common 3D formats including OBJ, FBX, GLTF, GLB, DAE, and STL
- Camera controls with keyboard and mouse interaction
- Scene hierarchy and property panel layout ready for future editor features
- macOS native embedding path for integrating the renderer inside Electron window content
- CMake-based build pipeline for the native engine

## Tech Stack

- Electron
- JavaScript / TypeScript-ready UI layer
- C++17
- OpenGL
- GLFW
- GLM
- Assimp
- CMake

## Project Structure

```text
archrenderer/
├── electron/             # Electron application shell
│   ├── main.js
│   └── preload.js
├── engine/               # Native engine and CMake build files
│   ├── CMakeLists.txt
│   ├── build/            # Generated build artifacts
│   ├── glad/            # OpenGL loader files
│   ├── include/         # Engine headers
│   ├── shaders/         # GLSL shader files
│   └── src/             # C++ renderer source
├── frontend/             # UI HTML/CSS/JS for the viewer
│   ├── index.html
│   ├── renderer.js
│   └── style.css
├── models/               # Sample 3D model assets
├── textures/             # Texture assets
├── package.json          # Electron app metadata and scripts
├── backup.txt            # Project backup notes
├── bacjup_shaders.txt    # Shader backup notes
└── README.md             # Project documentation
```

## Prerequisites

Before running the project, install the following:

- Node.js 18+
- npm
- CMake 3.20+
- A C++ compiler
  - On macOS: Xcode Command Line Tools
- OpenGL development libraries
- GLFW, GLM, and Assimp development packages

On macOS, the project also uses Homebrew commonly for native dependencies:

```bash
brew install cmake glfw glm assimp
```

## Getting Started

Clone the repository:

```bash
git clone https://github.com/sinner-alive/ArchRenderer.git
cd ArchRenderer
```

Install the Electron app dependencies:

```bash
npm install
```

Start the app:

```bash
npm start
```

This launches the Electron desktop shell and builds or invokes the native rendering engine as needed.

## Building the Native Engine

You can build the engine directly with CMake:

```bash
cmake -S engine -B engine/build
cmake --build engine/build --parallel 4
```

On macOS, the project also builds a Node native addon target named `ArchRenderNative.node` for the Electron integration layer.

## Controls

The viewer supports basic interactive navigation:

- `W`, `A`, `S`, `D` — move camera
- `Q`, `E` — adjust vertical camera motion
- Mouse movement — orbit/turn camera
- Mouse scroll — zoom in/out
- `Esc` — close the window

## Roadmap

The project is currently structured as a foundation for a richer architectural editor and viewer. Planned areas of expansion include:

- better scene manipulation tools
- transform controls for selected objects
- material and lighting controls
- asset browser and project organization
- improved scene hierarchy management
- import/export improvements for broader CAD / BIM workflows

## License

This project currently uses the ISC license as defined in `package.json`.

## Contributing

Contributions are welcome. If you want to improve the renderer, add new tools, or expand the architecture workflow, please open an issue or submit a pull request with a clear description of the change.

## Notes

This repository currently includes generated build artifacts under `engine/build/` and sample model assets under `models/` and related directories. Those are useful for local testing and development, but you should treat generated output as build-time artifacts rather than source-of-truth project code.
