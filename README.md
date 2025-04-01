# KuchCraft2

KuchCraft2 is a **Minecraft clone** written in C++ using an **Entity Component System (ECS)**. The project aims to create an optimized and extensible voxel-based game using modern rendering techniques. It is designed as a learning experience and a professional portfolio project, incorporating advanced game development principles.

## Features
### Core Gameplay
- **Voxel-based world** – Similar to Minecraft, the world consists of blocks that can be placed and destroyed.
- **Infinite Procedural Terrain** – Terrain is generated dynamically using chunk-based systems.
- **Custom Chunk Management** – Efficient handling of world data using chunk-based storage to improve performance.

### Rendering
- **OpenGL-based Rendering** – Uses GLFW and GLAD for high-performance 3D rendering.
- **Custom Shader System** – Features a preprocessor for dynamic shader configuration.
- **Frustum Culling and Optimization** – Prevents unnecessary rendering of unseen chunks.

### Performance
- **Entity Component System (ECS)** – Modular architecture for efficient object management.

### Input & UI
- **Event-driven Input Handling** – Uses GLFW callbacks for keyboard, mouse, and window events.
- **ImGui Debugging Interface** – Real-time debugging and performance monitoring.

### Data & Configuration
- **JSON Serialization** – Uses Nlohmann JSON for saving and loading game data.

## Build
Clone the repository with `git clone --recursive https://github.com/wooodiest/KuchCraft2`

Project tested in Visual Studio 2022, currently works only for Windows. To generate the project run `GenProject` found in `scripts` folder.

## Roadmap / Future Plans
### Gameplay Improvements
- **Physics System** – Collision detection, gravity simulation, and rigid-body physics.
- **Survival Mechanics** – Health, hunger, crafting, and inventory management.
- **Advanced Terrain Features** – Biomes, caves, and dynamic terrain updates.

### Rendering Enhancements
- **Lighting System** – Pretty cool lighting.
- **More Advanced Chunk Optimization** – Meshing, occlusion culling, and LOD rendering.

### Modding
- **Modding API** – Lua scripting or plugin support for user-created content.

### World Persistence
- **Efficient Save/Load System** – Storing world state to disk using binary formats.

