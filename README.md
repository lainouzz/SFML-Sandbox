# Sand Sandbox

A real-time cellular automata physics simulation built with **C++17** and **SFML**. The project simulates multiple interacting materials — sand, water, lava, fire, and more — on a 2D grid, with each cell governed by its own physics rules updated every frame.

---

## 📸 Overview

| Feature | Detail |
|---|---|
| Window Size | 800 × 600 px |
| Grid Resolution | 200 × 150 cells (4 px per cell) |
| Target Framerate | 60 FPS |
| Language | C++17 |
| Graphics Library | SFML | C++17 |

---

## 🧱 Materials

| Key | Material | Behaviour |
|---|---|---|
| `1` | **Sand** | Falls downward, slides diagonally |
| `2` | **Wet Sand** | Falls like sand but heavier |
| `3` | **Stone** | Static; acts as a solid wall |
| `4` | **Wood** | Static; can be ignited by fire or lava |
| `5` | **Water** | Mass-based fluid simulation; flows and spreads |
| `6` | **Lava** | Fluid like water; ignites adjacent wood cells |
| `7` | **Fire** | Spreads heat, burns away over time, ignites wood |

---

## 🎮 Controls

| Input | Action |
|---|---|
| `Left Click` | Place selected material |
| `Right Click` | Erase cells under cursor |
| `Scroll Wheel` | Increase / decrease brush radius (1 – 15) |
| `1` – `7` | Select material |
| `C` | Clear the entire grid |
| `O` | Toggle overwrite mode (draw over existing cells) |
| `Enter` (Main Menu) | Start simulation |
| `Esc` (Main Menu) | Exit application |

---

## 🗂️ Project Structure

```
SMFLPhysics/
├── SMFLPhysics.cpp   # Entry point — window, game loop, state management
├── Constants.h       # Grid dimensions, cell size, shared globals
├── Cell.h            # Cell struct, MaterialType enum, palette, brush helpers
├── Grid.h / .cpp     # Grid initialisation and clear utilities
├── Physics.h / .cpp  # Per-material physics update functions
├── Input.h / .cpp    # Keyboard, mouse, and scroll event handling
├── Renderer.h / .cpp # Pixel-level image rendering from grid state
├── UI.h / .cpp       # HUD text (FPS, grain count, brush size, instructions)
├── MainMenu.h / .cpp # Main menu screen rendering
```
Please note that there is a small type in SMFLPhysics.cpp, to lazy to change :P
---

## ⚙️ Architecture

### Game Loop
The main loop runs at 60 FPS and switches between two `GameState` values:
- **`MainMenu`** — renders the title screen, waits for `Enter`.
- **`Playing`** — processes input, runs physics, renders the grid, and draws the HUD.

### Grid
The simulation world is a `std::vector<std::vector<Cell>>` of size `GRID_HEIGHT × GRID_WIDTH`. Each `Cell` stores:
- `type` — the material
- `filled` / `burning` — state flags
- `mass` — used by fluid and fire systems
- `temperature` — drives ignition of wood
- `velocityX / Y` — reserved for future velocity-based movement
- `color` — per-cell colour with slight random variation for a natural look

### Physics Systems
Each system iterates the grid once per frame:

- **`updateSandPhysics`** — scans bottom-up; moves sand/wet-sand down or diagonally if the cell below is empty.
- **`updateFluidPhysics`** — alternates left-to-right and right-to-left scans to avoid directional bias; delegates per-cell logic to `fluidCellUpdate`.
- **`fluidCellUpdate`** — mass-transfer fluid model: flow down first, then horizontally, then upward (for pressure); lava additionally heats adjacent wood.
- **`updateFirePhysics`** → **`fireCellUpdate`** — reduces fire mass each tick (burn rate), spreads heat to all 8 neighbours, ignites wood that reaches ignition temperature, and removes the cell when mass reaches zero.

### Rendering
`Renderer` writes cell colours directly into an `sf::Image`, uploads it to an `sf::Texture`, and draws the resulting `sf::Sprite` scaled by `CELL_SIZE`. Empty cells are transparent over a solid background clear colour.

---

## 🔭 Planned Features

- 🔥 Full temperature propagation system
- 💧 Pressure-based fluid simulation
- 💥 Explosions and destruction
- 💨 Smoke visual effect
- 🪨 Coal remnants after fire burns out
- 💾 Save / load grid state
- 🖌️ Improved brush shapes and UX

---

## 🛠️ Building

1. Install [SFML](https://www.sfml-dev.org/) and link it to the project.
2. Open `SMFLPhysics.vcxproj` in Visual Studio.
3. Set the build configuration to **Debug** or **Release** (x64).
4. Build and run (`F5`).

> **Note:** The font path in `SMFLPhysics.cpp` is currently hardcoded. Update it to point to a valid `ARIAL.TTF` on your machine, or place the font at `x64/Debug/font/ARIAL.TTF` relative to the project root.

---

## 📄 License

This project is for portfolio and educational purposes. This ReadMe was created with the help of AI to speed things up and make it look nicer :)
