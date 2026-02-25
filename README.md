<p align="center">
    <img width="400" alt="Screenshot of Menuscreen" src="https://github.com/user-attachments/assets/44cafe16-bc5c-4e44-aa1d-ab1d8e773e4c" />
    <img width="400" alt="Screenshot of gameplay" src="https://github.com/user-attachments/assets/10399d24-1989-4f96-99dc-16b841060584" />
</p>

---

[![C++](https://img.shields.io/badge/C++-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-%238CC445.svg?style=flat&logo=sfml&logoColor=white)](https://www.sfml-dev.org/)
[![CMake](https://img.shields.io/badge/CMake-%23064F8C.svg?style=flat&logo=cmake&logoColor=white)](https://cmake.org/)

Master the skies in this high-octane, SFML-powered aerial evasion thrill-ride.

---

## 📖 Contents
1. [Tech Stack](#-tech-stack)
2. [Engine Architecture](#-engine-architecture)
3. [Gameplay Mechanics](#-gameplay-mechanics)
4. [Build & Setup](#-build--setup)

---

## 🏗 Tech Stack

- **Core:** C++17
- **Graphics & Audio:** SFML 2.6+ (Simple and Fast Multimedia Library). SFML is included directly via git submodules to ensure exact version parity and self-contained builds.
- **Build System:** CMake

### Project Structure
```text
SkyRage/
├── external/         # Git submodules (SFML source)
├── assets/           # Textures (.png), Audio (.ogg), and Fonts (.ttf)
├── include/          # Header files (.h)
└── src/              # Source files (.cpp)
    ├── Components/   # Game entities (PlayerPlane, EnemyPlane, Background)
    ├── Core/         # Engine singletons (State, Event, Audio, Texture Managers)
    └── States/       # Game screens (Menu, Play, Pause, Settings, Win, GameOver)
```

---

## 🗄 Engine Architecture

The game avoids a monolithic game loop by implementing a robust, modular engine core. Game components interface via singleton managers to isolate asset loading and event dispatching logic.

### 1. State Machine (`StateManager.cpp`)
The application is controlled by a stack-based State Manager. States interface with the engine via virtual `update()`, `render()`, `init()`, and `exit()` functions.

| State | Responsibility | Logic |
| :--- | :--- | :--- |
| `MenuState` | Title screen and main navigation. | Select "Start Game" or "Settings". |
| `SettingsState` | Configuration of game parameters. | Modifies map textures (Day/Night/Sunset) and difficulty variables. |
| `PlayState` | The core gameplay loop. | Handles the countdown, updates entities, checks collisions, and increments distance limit. |
| `PauseState` | Freezes the `PlayState` beneath it. | Halts background music and renders an obsidian overlay over the frozen frame. |
| `GameOverState` | Loss condition handling. | Triggered by collisions. Reverts control to menu or re-initializes `PlayState`. |

Because the State Manager is a stack, pausing the game simply pushes `PauseState` onto the stack. The engine only updates the top state but renders down the stack.

### 2. Resource Management
File IO is centralized natively to prevent blocking the render thread.
- **`TextureManager` / `FontManager`:** Caches `.png` and `.ttf` assets into memory during initialization mapped by Enum (`TextureID`, `FontID`).
- **`SoundManager`:** Loads `sf::SoundBuffer` data into memory and executes playback commands asynchronously.

---

## ⚡ Gameplay Mechanics

### Hyper-Speed Dynamics
Movement scaling relies on a delta-time acceleration framework.
- **Environmental Scrolling:** The background translates downward based on the calculated forward vector of the player: `scrollOffset -= (player_velocity * constant + scrollSpeed) * dt`.
- **Relative Entity Spawning:** `EnemyPlane` vertical velocity is a function of the active player speed (`BaseSpeed + (PlayerSpeed * Scalar)`). As the user accelerates towards `MAX_VELOCITY`, intercept rates increase linearly.

### Collision Precision
Top-down sprite bounding boxes (`sprite.getGlobalBounds()`) often register intersections on transparent texture regions, specifically wingtips.
- **Refined Bounds:** `PlayerPlane` and `EnemyPlane` apply a 0.7x (70%) uniform scalar reduction to their generated `sf::FloatRect` during collision checks (`intersects()`). This ensures crash conditions are only triggered by central fuselage overlap.

---

## 🛠 Build & Setup

Because this project utilizes **Git Submodules**, there is no requirement to install SFML binaries system-wide. We provide automated scripts for Linux and Windows compilation.

### 1. Initialize Source Control
Ensure the SFML submodule repository is cloned and initialized:
```bash
git submodule update --init --recursive
```

### 2. Automated Compilation

**🪟 Windows (Zero Dependencies)**
Compilation on Windows requires no external package installations. SFML bundles its exact required `extlibs` within the submodule.
Execute the following batch script from Command Prompt or PowerShell. The script programmatically resolves whether the host utilizes MSVC (`cl.exe`) or MinGW (`gcc.exe`) to configure the CMake Generator.
```cmd
build.bat
```

**🐧 Linux / macOS**
Ensure the host natively provides `build-essential` and `cmake`. (If compiling bare-metal SFML, installing `libudev-dev`, `libfreetype6-dev`, and generic x11/opengl development headers via Apt/Pacman may be required on fresh installations).
```bash
chmod +x build.sh
./build.sh
```

**🐳 Docker Container (Linux)**
To bypass base library dependencies (`libudev`, etc.) entirely, compile via the provided Ubuntu Docker environment. The container resolves all apt-level requirements, mounts the directory, executes CMake, and writes the resulting binary to the host volume.
```bash
chmod +x build_docker.sh
./build_docker.sh
```

### 3. Execution
The success output from the automated build scripts ensures the active executable is placed in the designated directory alongside the synced assets.

**Windows:**
```powershell
.\build\bin\Release\SkyRage.exe   # Produced via MSVC
.\build\bin\SkyRage.exe           # Produced via MinGW
```

**Linux / macOS:**
```bash
./build/bin/SkyRage
```

---
Built with ❤️ — SKY RAGE
