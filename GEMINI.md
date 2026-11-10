# Project Rules: CO1301 Games Concepts (Spheres.io — TL-Engine 11)

## Environment & Build Configuration
- **Engine**: TL-Engine 11 (`C:\Users\Public\Documents\TL-Engine11\`)
- **Compiler / MSBuild**: Visual Studio 18 Insiders (`v145` toolset, `C++20`, Windows SDK `10.0.26100.0`)
- **Build Command (Debug x64)**:
  ```powershell
  & "C:\Program Files\Microsoft Visual Studio\18\Insiders\MSBuild\Current\Bin\amd64\MSBuild.exe" "TL11-Template.vcxproj" /p:Configuration=Debug /p:Platform=x64 /v:minimal
  ```
- **Launch Command**:
  ```powershell
  Start-Process -FilePath ".\TL11-TemplateDebug.exe" -WorkingDirectory "."
  ```
  *(Ensure working directory is the workspace root so `Shaders/`, `media/`, and runtime `.dll` files are resolved properly.)*

## Coursework & Readable Code Rules (MANDATORY)
1. **Single-File Architecture & Lab 1–4 Techniques Only**:
   - All game code must remain in `TL11-Template.cpp` (or the final `Assignment1_[Username].cpp`).
   - Only standard C++ headers (`<iostream>`, `<sstream>`) and `"TL-Engine11.h"` (`using namespace tle;`) are allowed.
   - Never use techniques from outside Lab 1–Lab 4 (no custom `struct`/`class`, no `std::vector`, no lambdas).
2. **Minimize `->` Arrow Operators**:
   - Store model coordinates in plain `float` variables and arrays (`playerX`, `playerZ`, `itemX[]`, `itemZ[]`, `enemyX[]`, `enemyZ[]`) so math and collision functions (`GetDistance`, `SphereToBox`) use **zero `->` operators**.
   - Only use `->` when calling TL-Engine methods directly (`myEngine->...`, `player->MoveLocalZ(...)`, `myFont->Draw(...)`).
3. **No Chained `||`, `&&`, or `? :` Operators**:
   - Never write `||`, `&&`, or ternary `? :` on one line. Always write separate vertical `if` blocks so every condition is easy to read top-to-bottom.
4. **Full Descriptive Variable Names (No Single-Letter `i` or `e` Counters)**:
   - Never use single-letter loop variables like `i` or `e` or cryptic abbreviations (`px`, `dx`). Use clear names like `itemNum`, `enemyNum`, `otherItem`, `vectorX`, `vectorZ`, `distance`.
5. **No Magic Numbers, Minimal Clutter**:
   - Define clear `const` variables at the top of the file for speeds, bounds, spawn ranges, radii, and points (`kSphereSpeed`, `kRotationSpeed`, `kSpawnRange`, `kSpawnMin`, etc.).
   - Use whole numbers (`0`, `-5`, `10`, `90`) where the brief uses whole numbers; only use `.5f` or `.2f` where an actual decimal is required (`2.5f`, `1.2f`).
6. **Short 2–4 Word Comments**:
   - Keep comments short and plain English (`// Load meshes`, `// Move player`, `// Check water`).
7. **NEVER Use ASCII Art Diagrams**:
   - Always use Mermaid diagrams, Markdown tables, or HTML/SVG in `visual_guide.html`.
