# Learner Evidence Model: Profile & Mastery Frontier

This document tracks the human developer's verified conceptual mastery, coding preferences, resolved misconceptions, and active frontier.

---

## 1. Developer Profile & Interaction Invariants

| Attribute | Value / Rule |
| :--- | :--- |
| **Course & University** | UCLan Cyprus — CO1035 / CO1301 Games Concepts (2026-27) |
| **Development Toolchain** | Visual Studio 2026 Professional (v145), Windows 11, TL-Engine 11 |
| **Communication Style** | **Concise technical transfer**: no fluff, no repetitive congratulations, no obvious code comments. |
| **Visual Invariant** | **Mandatory diagrams**: every spatial, coordinate, state machine, or geometric delta must include a visual diagram (Mermaid / ASCII). |
| **Agency Rule** | **Student writes all code**: The student must type the code into Visual Studio and run builds (`Ctrl+Shift+B`, `F5`). AI provides minimal conceptual deltas. |

---

## 2. Demonstrated Competencies (Mastered Concepts)

1. **Island Dimensions & Coordinate Extents**:
   - Understands that a $200 \times 200$ island centered at origin spans $X \in [-100, +100]$ and $Z \in [-100, +100]$.
2. **Center Pivot & Ground Flush Alignment**:
   - Mastered the vertical alignment formula: $\text{Model } Y = \text{Ground } Y + \text{Radius / Half-Height}$.
   - Correctly understands why sphere rests at $Y = 10.0$ and cube at $Y = 2.5$ on ground $Y = 0.0$.
3. **Coding Standards & Naming Conventions**:
   - Understands why constants use lowercase `k` prefix (*Konstant*) rather than `c`.
   - Adheres to declaring constants at the top of the file outside `main()`.
4. **Engine Initialization Order**:
   - Corrected prior sequence error: `New3DEngine(TL11)` must execute before meshes are loaded.
5. **Asset Customization**:
   - Successfully loaded and applied custom skins (`tiles1.jpg` for chessboard island, `Stars.jpg` for cosmic space water).

---

## 3. Resolved Misconceptions

| Initial Misconception | Grounded Resolution | Verification Method |
| :--- | :--- | :--- |
| Tried `#include "MyEngine"` | Explained that TL-Engine is included via `<TL-Engine.h>` and `using namespace tle;`. | Resolved in code. |
| Placed mesh loading before engine creation | Diagrammed the factory pattern: the engine instance must exist to construct meshes. | Reordered initialization in `TL11-Template.cpp`. |
| Assumed models should be deleted on pickup | Clarified that deleting models causes dangling pointer crashes; hiding via `SetY(-100.0f)` is the canonical pattern. | Understood and implemented. |

---

## 4. Active Frontier (Current Step)

- **Current Task**: Integrating boundary checking and cube pickup detection in `TL11-Template.cpp`.
- **Next Step**: Verifying Milestone 1 in runtime (`F5`).
- **Immediate Frontier**: Milestone 2 dual cameras, score tracking HUD, and sphere scaling math.
