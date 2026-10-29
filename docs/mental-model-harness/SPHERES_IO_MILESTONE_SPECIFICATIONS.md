# Spheres.io: Milestone Specifications & Rubric Contracts

This document contains the frozen requirements, technical specifications, and acceptance tests for the **CO1035 / CO1301 Games Concepts** coursework: **Spheres.io**.

---

## Marking Rubric Overview

| Grade Band | Percentage | Primary Technical Requirements |
| :--- | :--- | :--- |
| **Pass** | **40% - 49%** | Milestone 1: Island, water, skybox, player sphere, 4 minicubes. Sphere movement (W/S forward/backward, A/D rotate), Top-down camera pan, P key pause/unpause, water boundary check, minicube collection. |
| **Lower Second (2:2)** | **50% - 59%** | Milestone 2: Dual camera views (Top-down key `1`, Isometric key `2`), score display (10 pts/cube) using TL-Engine font/text, sphere growth (scale 1.2x after 4 cubes with vertical pivot compensation). |
| **Upper Second (2:1)** | **60% - 69%** | Milestone 3: Enemy chaser sphere, vector calculation towards player, unit vector normalization, variable delta speed, collision resolution (game over or absorption), minicube respawning in valid island bounds. |
| **First Class (1st)** | **70% - 79%** | Milestone 4: Chase camera (key `3`) tracking sphere position and orientation smoothly, state machine (`Ready`, `Playing`, `Paused`, `GameOver`, `Won`), restart functionality (`R` key), game reset. |
| **High First (1st+)** | **80% - 100%** | Milestone 5: Advanced AI behaviors (multiple enemies: Chaser, Wanderer, Evader), physics friction/momentum, hypercube power-ups, exceptional code quality, zero warnings, detailed report. |

---

## Milestone 1: The Core Foundation (Pass - 40%)

### 1. Requirements
1. **Scene Setup**:
   - Island: `island.x` at `(0, -5, 0)` (Size $200 \times 200$, bounds $X \in [-100, 100]$, $Z \in [-100, 100]$).
   - Water: `water.x` at `(0, -5, 0)`.
   - Skybox: `sky.x` at `(0, -1000, 0)`.
   - Player Sphere: `spheremesh.x` at `(0, 10, 0)` (Radius $R = 10$).
   - 4 Minicubes: `minicube.x` placed at 4 island locations at $Y = 2.5$ (Cube size $5 \times 5$, half-height $2.5$).
2. **Player Controls**:
   - `W`: Move forward along local $Z$: `player->MoveLocalZ(kPlayerSpeed * frameTime)`.
   - `S`: Move backward along local $Z$: `player->MoveLocalZ(-kPlayerSpeed * frameTime)`.
   - `A`: Rotate left (yaw counter-clockwise): `player->RotateY(-kTurnSpeed * frameTime)`.
   - `D`: Rotate right (yaw clockwise): `player->RotateY(kTurnSpeed * frameTime)`.
3. **Camera Controls**:
   - Top-down camera at `(0, 200, 0)` with local pitch rotation $90^\circ$.
   - Arrow keys (`Up`, `Down`, `Left`, `Right`) pan camera in world $X/Z$.
4. **Game State**:
   - `P` key toggles between `Playing` and `Paused` (using key hit / edge-trigger logic to prevent rapid flickers).
5. **Water Boundary Check**:
   - If player $|X| > 100$ or $|Z| > 100$, state transitions to `GameOver` and movement halts.
6. **Cube Pickup**:
   - When player sphere center is within distance $(R_{\text{sphere}} + \text{HalfCube})$ of cube, cube becomes inactive.
   - Collected cube is hidden below ground (`model->SetY(-100.0f)`).

### Acceptance Tests
- [ ] Sphere turns smoothly with A/D and drives forward/backward with W/S.
- [ ] Panning with Arrow keys moves camera across island.
- [ ] Pressing P freezes sphere movement; pressing P again resumes.
- [ ] Driving sphere off the edge into water immediately stops control and triggers Game Over.
- [ ] Driving over any of the 4 minicubes immediately removes the cube.

---

## Milestone 2: Cameras, HUD & Growth (2:2 - 50%)

### 1. Requirements
1. **Camera Modes**:
   - Key `1`: Top-down camera (`Y = 200`, rotated $90^\circ$ pitch down).
   - Key `2`: Isometric 45-degree angle camera (`Y = 140`, `Z = -140`, rotated $45^\circ$ pitch up/down).
2. **Scoring & Text HUD**:
   - Track `score` integer (starts at 0, $+10$ points per collected minicube).
   - Display on screen using TL-Engine font object (`IFont* font = myEngine->LoadFont("Comic Sans MS", 36)` or TL11 font API).
   - Render text: `"Score: " + std::to_string(score)` during active frame.
3. **Player Growth**:
   - When 4 minicubes are collected:
     - Scale sphere by $1.2\times$: `player->Scale(1.2f)`.
     - Update radius: $R_{\text{new}} = R_{\text{old}} \times 1.2 = 12.0$.
     - Adjust vertical height: `player->SetY(12.0f)` so bottom remains flush at $Y = 0$.

### Acceptance Tests
- [ ] Pressing '1' switches to top-down view; pressing '2' switches to isometric 45° view.
- [ ] HUD displays score counter, updating by +10 each time a cube is picked up.
- [ ] Collecting the 4th cube visibly scales the sphere up by 20% without sinking through the floor.

---

## Milestone 3: Chaser AI, Vectors & Respawn (2:1 - 60%)

### 1. Requirements
1. **Chaser Sphere**:
   - Load second sphere mesh / skin (e.g. `enemysphere.jpg` or `hypersphere.jpg`).
   - Spawn at opposite corner of island at $Y = 10$.
2. **Vector Math & Chaser Navigation**:
   - Vector to player: $\vec{V} = (P_x - E_x, \; 0, \; P_z - E_z)$.
   - Distance: $D = \sqrt{dx^2 + dz^2}$.
   - Unit direction: $\hat{V} = (\frac{dx}{D}, \; 0, \; \frac{dz}{D})$.
   - Movement: $E_x \mathrel{+}= \hat{V}_x \times \text{Speed}_E \times \text{frameTime}$, $E_z \mathrel{+}= \hat{V}_z \times \text{Speed}_E \times \text{frameTime}$.
   - Chaser sphere faces direction of motion (`chaser->LookAt(player)` or trigonometry).
3. **Sphere-to-Sphere Collision**:
   - Check if $\text{DistSq}(P, E) \le (R_P + R_E)^2$.
   - If player is smaller: Game Over (player eaten).
   - If player is larger: Enemy eaten, $+50$ points.
4. **Cube Respawn System**:
   - Once all 4 cubes are collected, spawn a new batch in randomized coordinates within bounds $[-80, 80]$.

### Acceptance Tests
- [ ] Chaser sphere relentlessly tracks and closes distance toward player.
- [ ] Movement speed is frame-rate independent.
- [ ] If chaser touches player, collision triggers game over.
- [ ] Picking up all cubes triggers new respawn wave.

---

## Milestone 4: Chase Camera & Game Lifecycle (1st - 70%)

### 1. Requirements
1. **Chase Camera (Third-Person / Key `3`)**:
   - Positioned behind and above the player sphere along the player's backward vector:
     $$\text{CamPos} = \text{PlayerPos} - (\text{PlayerForward} \times \text{FollowDist}) + (0, \text{CamHeight}, 0)$$
   - Rotates to match player facing direction.
2. **State Machine (`EGameState`)**:
   - `Ready`: Title screen, instructions `"Press SPACE to Start"`.
   - `Playing`: Active gameplay, timer running, score updating.
   - `Paused`: Movement halted, `"PAUSED - Press P to Resume"`.
   - `GameOver`: Loss state, `"GAME OVER - Press R to Restart"`.
   - `Won`: Win state (e.g., reached target score), `"YOU WIN! - Press R to Restart"`.
3. **Full Reset Logic**:
   - Pressing `R` in `GameOver` resets player position to `(0, 10, 0)`, resets scale to $1.0$, resets score to 0, respawns cubes, resets chaser.

### Acceptance Tests
- [ ] Key '3' locks camera behind player sphere and turns dynamically as player turns.
- [ ] Game boots into Ready state; pressing Space starts game.
- [ ] Pressing R after death cleanly resets everything without crashes or leaks.

---

## Milestone 5: Polish, Multi-AI & Physics (High 1st - 80-100%)

### 1. Requirements
1. **Multiple AI Types**:
   - **Chaser**: Directly hunts player.
   - **Wanderer / Roamer**: Bounces around island borders, patrols waypoints.
   - **Evader**: Runs away from player if player is larger.
2. **Physics / Momentum**:
   - Acceleration and deceleration friction instead of instant start/stop velocity.
   - Inertia on turning (banking or drift).
3. **Power-ups & Hypercube**:
   - Gold/Hyper cube spawns temporarily; gives speed boost or invulnerability for 5 seconds.
4. **Code Quality**:
   - Zero compilation warnings at `/W3` or `/W4`.
   - Clean, modular helper functions for vector math, collision, resets.
   - Zero magic numbers; 100% adherence to single translation unit.
