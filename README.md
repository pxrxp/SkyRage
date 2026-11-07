https://github.com/user-attachments/assets/56d8d91f-ce79-487b-9a39-ee229319dc1e

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![SFML](https://img.shields.io/badge/SFML-2.6%2B-8CC445?style=for-the-badge&logo=sfml&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.10%2B-064F8C?style=for-the-badge&logo=cmake&logoColor=white)

SKY RAGE is a top-down plane game written in C++20 with SFML. Steer across the
screen, accelerate to cover distance faster, and avoid the planes coming
toward you. A collision ends the run; reach the end of the distance meter to
win.

This is a game project, not a separate reusable engine. Its code is divided
into game components, screen states, and a small set of shared managers for
the window, events, assets, and audio.

## Playing

The main menu offers Play, How to Play, Leaderboard, and Settings. A run
begins with a countdown. Your plane starts near the bottom of the screen and
can move left and right; it cannot leave the horizontal play area. Hold Up to
accelerate. Speed helps reduce the remaining distance faster, but enemy planes
also move down faster as your speed increases.

The game tracks elapsed time and remaining distance. Avoid the incoming
planes until the distance reaches zero. Colliding with one ends the run.
Completed runs are written to a local high-score file.

Settings lets you choose among three sky backgrounds. The selection is saved
locally, so it remains selected the next time you launch the game.

## Controls

| Input | Action |
| --- | --- |
| `A` / Left arrow | Steer left |
| `D` / Right arrow | Steer right |
| `W` / Up arrow | Accelerate |
| `Esc` | Pause during a run |
| `Esc` | Resume from the pause screen |
| `Ctrl` + `Esc` | Leave the paused run and return to the menu |
| `Up` / `Down` | Select menu items; cycle environments in Settings |
| `Enter` | Activate the selected menu item |
| Left mouse button | Select a menu item |
| `F11` | Toggle fullscreen |

## What happens during a run

`PlayState` owns the player, scrolling background, enemy spawner, countdown,
HUD, and run state. Each update uses SFML's elapsed frame time to advance the
simulation.

### Player movement

`PlayerPlane` starts horizontally centered, at 80% of the window height. Its
position and size are scaled relative to the window, and its horizontal
movement is restricted to the range from 5% to 95% of the window width.

Steering moves the plane sideways and banks its sprite toward the direction
of travel. Holding Up increases forward velocity; releasing it returns the
plane toward its target speed.

| Value | Current setting |
| --- | --- |
| Target forward velocity | `1.0` |
| Maximum forward velocity | `8.0` |
| Acceleration | `2.0` velocity units per second |
| Deceleration toward target | `1.0` velocity unit per second |
| Maximum bank angle | `15°` |

The exhaust sprite appears while accelerating. Its size pulses over time and
it follows the plane's tail.

### Enemy planes

`EnemySpawner` checks for a new spawn every 0.6 seconds. Each enemy appears
at the top of the screen at a random horizontal position between 5% and 95%
of the window width. Enemies move down and are removed after they pass 120%
of the window height.

Their downward speed is calculated from the window height and the player's
current velocity:

```text
enemy speed = window height × 0.3
            + player velocity × window height × 0.5
```

That ties the obstacle motion to the player's acceleration: flying faster
advances the run, but also brings obstacles down faster.

### Background and distance

The background texture repeats vertically and scrolls continuously. Its
scroll rate combines a base scroll speed with the player's velocity:

```text
scroll rate = 200 + player velocity × 160
```

The HUD shows elapsed time, remaining distance, and a progress bar. The run
ends in a win when remaining distance reaches zero. A collision with an enemy
ends it in a loss.

Player and enemy collision rectangles are centered inside their sprite bounds
and reduced to 70% of the sprite's width and height. This leaves transparent
parts around the plane artwork out of the collision test.

**Current implementation note:** `PlayState` initializes remaining distance
to 100,000, but the progress bar uses 1,000,000 as its maximum distance.
Consequently, the bar starts about 90% full. The values are documented as
implemented rather than describing the bar as if it starts empty.

## Screens and game flow

The screens are implemented as states. Each state has `init`, `update`,
`render`, event handling, and `exit` methods.

| State | What it does |
| --- | --- |
| `MenuState` | Main menu. Buttons can be selected with the keyboard or mouse. |
| `HowToPlayState` | Displays the in-game control and objective instructions. |
| `SettingsState` | Cycles through the three environment indices and saves the selection. |
| `HighScoresState` | Reads and displays saved run times. |
| `PlayState` | Runs the countdown, simulation, HUD, collision checks, and win condition. |
| `PauseState` | Pauses music and queued sound playback; offers resume or return to menu. |
| `GameOverState` | Shows the captured gameplay frame after a collision. |
| `WinState` | Shows the captured gameplay frame after completing the distance. |

Starting a game pushes `PlayState`. Pressing Escape during a run captures the
current frame and pushes `PauseState`; Escape removes the pause screen and
resumes play. `Ctrl`+`Esc` leaves the paused run and returns to the menu.
Collision and completion push their respective end screens, which also show
the captured frame beneath an overlay.

## How the code is organized

```text
CMakeLists.txt
build.sh                 Linux configure and build script
build.bat                Windows configure and build script
Dockerfile               Ubuntu build environment
build_docker.sh          Build through Docker
.gitmodules              SFML, Dear ImGui, and ImGui-SFML submodules
assets/                  Sprites, sky images, fonts, and audio
include/
  Components/            Player, enemy spawner, and background declarations
  Core/                  State, event, window, texture, font, and sound declarations
  States/                Game-screen declarations
  Util/                  Executable-path helper declaration
src/
  main.cpp               Application loop and initial menu state
  Components/            Player movement, enemy spawning, background scrolling
  Core/                  Manager implementations
  States/                Menu, gameplay, settings, pause, and result screens
  Util/                  Platform-specific executable-path lookup
```

### Main loop and events

`src/main.cpp` creates the window and pushes the menu state. Its loop polls
SFML events, dispatches them through `EventManager`, updates the active state
using the elapsed frame time, and renders the state.

`EventManager` stores callbacks by state ID and SFML event type. A state
registers the events it needs when it is initialized and removes its
listeners when it exits. The manager dispatches events for the current state.

### State management

`StateManager` owns states in a stack of `std::unique_ptr<State>`. Pushing a
state calls its `init`; popping one calls its `exit`. Only the top state
receives updates and renders.

The pause, win, and game-over screens therefore do not keep the gameplay
state updating underneath them. Before pushing these screens, `PlayState`
renders the current game into an SFML render texture. The overlay states draw
that captured image behind their text and dimming rectangle.

### Assets and managers

The shared managers are singletons:

- **`WindowManager`** creates the SFML window, enables vertical sync, handles
  resize view changes, and toggles fullscreen with F11.
- **`TextureManager`** stores textures by `TextureID`; loaded textures use
  smooth filtering. The render texture used for screen captures is also
  stored here.
- **`FontManager`** stores fonts by `FontID`.
- **`SoundManager`** stores sound buffers by `SoundID` and has a queue serviced
  by an SFML thread. Music is handled separately with `sf::Music`.

The menu loads its background, fonts, and hover sound. Gameplay loads the
plane and exhaust textures, the selected sky texture, countdown and crash
sounds, and looping music. The game loop queues the countdown sounds, and a
collision queues the crash sound. Pausing pauses the music and queued sound
playback.

The repository also contains `engine.wav` and `game_over.ogg`; those files
are not currently loaded by the gameplay or menu code.

### Settings and high scores

`Util::getExecutablePath()` finds the directory containing the running
program on Windows, macOS, and Linux. Assets and the small data files are
resolved relative to that directory rather than the process's working
directory.

- `settings.txt` stores the selected environment as an integer from `0` to
  `2`. If the file is absent, the game uses environment `0`.
- `highscores.txt` stores one completion time per line. A time is appended
  when a run ends at zero remaining distance. The Leaderboard screen displays
  up to the first ten lines in the file; it does not sort the times.

## Building

### Requirements

- CMake 3.10 or later
- A C++20 compiler
- Git, to initialize the SFML and optional ImGui submodules
- The development libraries needed to build SFML for your platform

Initialize the submodules before building:

```bash
git submodule update --init --recursive
```

SFML is built from the `external/SFML` submodule by the top-level CMake
configuration. On Linux, SFML's graphics and audio dependencies must be
available to its build. The included Docker image installs a set of Ubuntu
22.04 build dependencies.

### Linux

```bash
./build.sh
./build/bin/SkyRage
```

`build.sh` configures the CMake project in `build/` and builds using the
available CPU cores. CMake copies `assets/` into `build/bin/assets` as part of
the build, next to the executable.

To build in the provided Ubuntu container:

```bash
./build_docker.sh
./build/bin/SkyRage
```

The Docker script mounts the repository into the container and leaves the
built files in the host's `build/` directory. Docker is used for compilation;
running the graphical game still requires a host display.

### Windows

Run `build.bat` from a command prompt where CMake and either MSVC or MinGW
are available:

```bat
build.bat
```

The script checks for MSVC first and otherwise tries MinGW. With MSVC, run:

```powershell
.\build\bin\Release\SkyRage.exe
```

With MinGW, run:

```powershell
.\build\bin\SkyRage.exe
```

### CMake directly

```bash
cmake -S . -B build
cmake --build build --config Release --parallel
```

When `DEV_PHASE` is defined at CMake configure time, the build also adds the
Dear ImGui and ImGui-SFML submodules and links ImGui-SFML into the executable.
The normal build does not enable that optional path.
