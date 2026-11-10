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

## Coursework & Coding Guidelines
- **Single-File Architecture**: All game code must remain in `TL11-Template.cpp` (or the final `Assignment1_[Username].cpp`).
- **No External Libraries**: Only standard C++ headers and `TL-Engine11.h` (`using namespace tle;`) are allowed.
- **No Magic Numbers**: Define named `const` variables for speeds, bounds, radii, timers, and gameplay values.

