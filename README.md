# CO1301 / CO1035 Games Concepts — Spheres.io (2026–27)

A 3D arcade game developed in C++ using **TL-Engine 11**, inspired by *Katamari Damacy* and `.io` arena games. Coursework 1 for module CO1035 / CO1301 (BSc Computing, UCLan Cyprus).

---

## Project Structure

```
├── TL11-Template.sln          # Visual Studio Solution
├── TL11-Template.vcxproj      # Visual Studio Project Configuration (v145 Toolset)
├── TL11-Template.cpp          # Single-File Game Source Code
├── media/                     # Game Media Assets (Meshes & Textures)
│   ├── spheremesh.x           # Player / NPC sphere mesh (Radius = 10)
│   ├── island.x               # Island ground (200x10x200)
│   ├── water.x                # Ocean boundary plane
│   ├── sky.x                  # Skybox mesh
│   ├── minicube.x             # Collectible cube mesh (5x5x5)
│   ├── hypercube.jpg          # Hyper Mode special cube skin
│   ├── hypersphere.jpg        # Player Hyper state skin
│   └── enemysphere.jpg        # NPC enemy sphere skin
├── .vscode/                   # VS Code Task & Launch Settings
├── sources/                   # Module Brief, Lecture & Lab Specifications
└── README.md
```

---

## Setup on Laptop (Visual Studio 2026 / Insiders)

### 1. Prerequisites
1. **Visual Studio 2026 (or Community / Professional)** with:
   - **Desktop development with C++** workload installed.
   - MSVC v145 toolset (or latest v143/v145).
   - Windows 11 SDK (10.0.26100.0 or higher).
2. **TL-Engine 11**:
   - Installed at default location: `C:\Users\Public\Documents\TL-Engine11\`.
   - Ensure `TL-Engine11.h` is present in `C:\Users\Public\Documents\TL-Engine11\include\`.

### 2. Building & Running
- **Visual Studio**:
  1. Open `TL11-Template.sln`.
  2. Select **Debug** and **x64**.
  3. Press **Ctrl + Shift + B** to build.
  4. Press **F5** to run.
- **VS Code**:
  1. Open project folder.
  2. Press **Ctrl + Shift + B** to trigger the MSBuild task.
  3. Press **F5** to launch debugger.

---

## Controls

| Key | Action |
| :--- | :--- |
| **W / S** | Move Sphere Forward / Backward |
| **A / D** | Rotate Sphere Left / Right |
| **Arrow Keys** | Pan Top-Down Camera (World X / Z) |
| **P** | Toggle Pause / Playing State |
| **1 / 2** | Toggle Top-Down (1) vs. Isometric (2) Camera |
| **Escape** | Exit Game |

---

## Academic Integrity & Submission
- Entire submission must be contained in a single `.cpp` file (`Assignment1_[Username].cpp`).
- Strictly zero external libraries outside the TL-Engine SDK.
