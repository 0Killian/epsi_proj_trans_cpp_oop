# Stardew

A 2D tile-based game engine prototype written in modern C++23, built with SFML. The project implements a complete scene management system, an efficient tilemap renderer, a thread pool for async loading, and a reference-counted resource registry.

## Features

- **Scene management** — abstract `Scene` base with `Init`, `Update`, `Render`, and `HandleEvent` lifecycle hooks
- **Tilemap system** — vertex-array-based renderer for large grids; custom binary `.htf` tilemap format
- **5 tile types** — Ground, Wall, PassagePoint (portals), Path (speed boost), Soil (tillable)
- **Camera** — pan and zoom with keyboard input (WASD / arrow keys, Q/Shift for zoom)
- **Thread pool** — multi-threaded task executor with OpenGL context sharing for async resource loading
- **Resource registry** — template-based manager with reference counting and automatic unloading
- **RNG** — seeded MT19937 generator for int, float, and double distributions

## Dependencies

| Library | How it's included |
|---|---|
| [SFML](https://github.com/SFML/SFML) | Git submodule |
| [spdlog](https://github.com/gabime/spdlog) | Headers bundled in `include/` |

## Requirements

- CMake 3.24.2+
- C++23 compiler (GCC 13+, Clang 16+, MSVC 2022+)
- Git

## Building

```bash
git clone --recursive https://github.com/0Killian/epsi_proj_trans_cpp_oop.git
cd epsi_proj_trans_cpp_oop

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

For a debug build (enables verbose spdlog output):

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

## Running

The executable expects to be run from the `working_directory/` folder so it can resolve asset paths:

```bash
cd working_directory
../build/Stardew          # Linux / macOS
../build/Release/Stardew.exe  # Windows
```

## Project Structure

```
epsi_proj_trans_cpp_oop/
├── CMakeLists.txt
├── include/            # Headers (game systems + spdlog)
├── src/
│   ├── main.cpp
│   ├── Application.cpp
│   ├── GameGrid.cpp
│   ├── MainMenuScene.cpp
│   ├── ThreadPool.cpp
│   ├── RandomNumberGenerator.cpp
│   └── tiles/          # One .cpp per tile type
├── scripts/
│   └── GenerateTilemap.cpp   # Standalone tilemap editor/generator
├── working_directory/
│   └── assets/
│       ├── textures/   # PNG assets
│       └── tilemaps/   # .htf binary tilemap files
└── SFML/               # Git submodule
```

## Tilemap Format (.htf)

Binary format with a fixed header followed by tile and entity data:

```
[width: u32][height: u32][tilesetPathSize: u32][tilesSize: u32][entitiesSize: u32]
[tilesetPath: char * tilesetPathSize]
[tiles: RawGameTile * N]
[entities: ...]           ← reserved, not yet parsed
```

Each tile entry:

```
[type: u8][size: u32][textureIndex: u32][data: u8 * (size - fixed)]
```

Use `scripts/GenerateTilemap.cpp` to create and edit `.htf` files interactively.

## Controls

| Key | Action |
|---|---|
| WASD / Arrow keys | Pan camera |
| Q | Zoom in |
| Shift | Zoom out |
| Escape | Quit |

## License

See [LICENSE](LICENSE).
