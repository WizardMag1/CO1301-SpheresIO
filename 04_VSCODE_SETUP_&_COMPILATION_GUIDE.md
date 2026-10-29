# VS Code Setup & C++ Compilation Guide for TL-Engine 11

## 1. Current System Status

- **Visual Studio Code**: Installed at `C:\Users\wizard\AppData\Local\Programs\Microsoft VS Code\Code.exe`.
- **TL-Engine 11**: Fully installed at `C:\Users\Public\Documents\TL-Engine11`.
- **Runtime Assets**: All required shaders, models, textures, and DLLs (`assimp-vc143-mt.dll`, `SDL2.dll`, `D3DCompiler_43.dll`, etc.) are placed directly in your workspace: `c:\Users\wizard\Documents\CO1301 Games Concepts (2026-27)`.
- **VS Code Configuration**: `.vscode/c_cpp_properties.json`, `.vscode/settings.json`, `.vscode/tasks.json`, and `.vscode/launch.json` are generated and ready.

---

## 2. Enabling the MSVC C++ Compiler for VS Code

TL-Engine is compiled as a native Windows C++ library (`TL-Engine11.lib`, v143 toolset). To compile and link your game in VS Code or MSBuild:

1. Launch the **Visual Studio Installer** already present on your PC:
   - Path: `C:\Program Files (x86)\Microsoft Visual Studio\Installer\vs_installer.exe`
2. Under "Available" or on your existing installation card, click **Modify** or **Install**.
3. Check the single box: **"Desktop development with C++"** (includes MSVC v143 C++ x64/x86 build tools and the Windows 10/11 SDK).
4. Click **Install / Modify** in the bottom-right corner.

*(Note: You do not need to install the full IDE if you prefer VS Code; installing just the C++ workload will enable `cl.exe` and `MSBuild.exe`, allowing VS Code to build seamlessly!)*

---

## 3. Building & Running Inside VS Code

### Option A: VS Code Build Task
- Open the workspace in VS Code:
  ```powershell
  code "c:\Users\wizard\Documents\CO1301 Games Concepts (2026-27)"
  ```
- Press `Ctrl + Shift + B` to trigger the build task (`Build with MSBuild (Debug x64)`).
- Press `F5` to start debugging with the external console window and 3D viewport.

### Option B: Developer Command Prompt / Terminal
You can also build directly from PowerShell / Terminal:
```powershell
& "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\MSBuild\Current\Bin\amd64\MSBuild.exe" TL11-Template.vcxproj /p:Configuration=Debug /p:Platform=x64
.\TL11-TemplateDebug.exe
```

---

## 4. Key Project Files Reference

- `TL11-Template.cpp`: Main C++ entry point and game loop.
- `TL11-Template.vcxproj`: MSBuild C++ project definition linking `TL-Engine11.lib`.
- `Shaders/`: Precompiled HLSL DirectX 11/12 shaders required for TL-Engine rendering.
- `assimp-vc143-mt.dll`, `SDL2.dll`, `D3DCompiler_43.dll`: Core dynamic link libraries required in the working directory when launching the executable.
