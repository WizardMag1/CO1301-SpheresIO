# TL-Engine 11 API Reference

This reference documents the modern TL-Engine 11 C++ API as defined in the official header files (`sources/tl-engine11_headers/*.h`). All types and functions reside within the `tle` namespace.

---

## 1. Engine Creation and Lifecycle (`TL-Engine11.h`, `3DEngine.h`)

### Engine Factory and Types
```cpp
namespace tle {
    enum EngineType {
        TL11,        // Modern TL-Engine 11 DirectX 11 engine
        TLXStandard, // Standard engine (formerly kTLX)
        TLXLegacy,   // Legacy variant (formerly kTLXLegacy)
        NumEngineTypes
    };

    TLEngine* New3DEngine(EngineType engineType);
}
```

### `class TLEngine` Lifecycle & Window Management
- `bool StartFullscreen(int width = 1920, int height = 1280);`
  Starts fullscreen 3D rendering. Can only be invoked once after engine creation.
- `void StartWindowed(int width = 1600, int height = 900);`
  Opens the 3D engine in a window of the given pixel dimensions. Can only be invoked once.
- `bool IsRunning();`
  Returns `true` while the engine is active; returns `false` when a stop request or window closure occurs.
- `bool IsActive();`
  Returns `true` if the engine window is the foreground active window. Input should be ignored if this returns `false`.
- `void Stop();`
  Requests the engine to terminate, causing subsequent calls to `IsRunning()` to return `false`.
- `void Delete();`
  Destroys engine internals and frees graphics memory. Must be called before program exit to prevent memory leaks.
- `int GetWidth();`
  Returns the width of the engine rendering window or display.
- `int GetHeight();`
  Returns the height of the engine rendering window or display.
- `int GetWindow();`
  Returns the native OS window handle.
- `void SetWindowCaption(const string& text);`
  Sets the title bar text of the window (no effect in fullscreen mode).

### Media Folder Management (`3DEngine.h`)
- `void AddMediaFolder(const string& folderName);`
  Adds a filesystem path to the media search list for loading meshes, skins, and fonts.
- `bool RemoveMediaFolder(const string& folderName);`
  Removes a previously added folder path from the search list.
- `void ClearMediaFolders();`
  Clears all registered media paths from the search list.

### Rendering & Timing (`3DEngine.h`)
- `void DrawScene(Camera* camera = nullptr);`
  Renders the complete 3D scene from the perspective of the specified camera (or most recently created camera if `nullptr`).
- `float FrameTime();`
  Returns the elapsed time in seconds between the two most recent calls to `DrawScene()`. Returns `1/60.0f` on initial frames. Instantaneous FPS equals `1.0f / FrameTime()`.
- `int AverageFPS();`
  Returns the rolling average frames per second measured over the last second.
- `void EnableVsync(bool enable = true);`
  Synchronizes frame presentation to monitor refresh rate (60Hz, 120Hz, etc.).
- `float Timer();`
  Returns the total running time in seconds since the engine was started.

---

## 2. Scene Graph Base Class (`SceneNode.h`)

`SceneNode` is the abstract base interface inherited by `Model`, `Camera`, and `Light`.

### Positioning
- `float GetX();` / `float GetY();` / `float GetZ();`
  Return current world-space X, Y, and Z coordinates.
- `void SetX(float x);` / `void SetY(float y);` / `void SetZ(float z);`
  Set world-space coordinate components individually.
- `void SetPosition(float x, float y, float z);`
  Sets world-space X, Y, and Z coordinates simultaneously.
- `float GetLocalX();` / `float GetLocalY();` / `float GetLocalZ();`
  Return local coordinates relative to the node's parent.
- `void SetLocalX(float x);` / `void SetLocalY(float y);` / `void SetLocalZ(float z);`
  Set local coordinates relative to the node's parent.
- `void SetLocalPosition(float x, float y, float z);`
  Sets local (X, Y, Z) coordinates relative to the parent node.

### World Movement
- `void MoveX(float x);` / `void MoveY(float y);` / `void MoveZ(float z);`
  Translates the node along world Cartesian axes.
- `void Move(float x, float y, float z);`
  Translates the node in world space by the specified 3D vector.

### Local Movement
- `void MoveLocalX(float x);` / `void MoveLocalY(float y);` / `void MoveLocalZ(float z);`
  Translates the node along its own rotated local axes.
- `void MoveLocal(float x, float y, float z);`
  Translates the node along its local axes by the specified vector.

### Rotation & Orientation
- `void RotateX(float x);` / `void RotateY(float y);` / `void RotateZ(float z);`
  Rotates the node around world Cartesian axes in degrees.
- `void RotateLocalX(float x);` / `void RotateLocalY(float y);` / `void RotateLocalZ(float z);`
  Rotates the node around its own local axes in degrees.
- `void LookAt(SceneNode* target);`
  Orients the node to face the target SceneNode.
- `void LookAt(float x, float y, float z);`
  Orients the node to face the given world coordinates.
- `void ResetOrientation();`
  Resets all rotation angles to default alignment.

### Scaling & Transform Matrix
- `void Scale(float scale);`
  Uniformly scales the node geometry by the specified factor.
- `void ScaleX(float x);` / `void ScaleY(float y);` / `void ScaleZ(float z);`
  Scales along individual coordinate dimensions.
- `void ResetScale();`
  Resets scale factors to 1.0.
- `void GetMatrix(float* matrixElts);`
  Extracts 16 float elements representing the 4x4 transform matrix.
- `void SetMatrix(float* matrixElts);`
  Applies 16 float elements to set the 4x4 transform matrix directly.

### Hierarchy & Physics
- `bool AttachToParent(SceneNode* parent);`
  Parents this node to another SceneNode. Local transformations become relative to the parent.
- `bool DetachFromParent();`
  Detaches this node from its parent, returning it to world space.
- `bool EnableCollisions(Model* collider, float gravity = -9.8f, float radiusX = 0.0f, float radiusY = 0.0f, float radiusZ = 0.0f);`
  Configures engine-level collision testing against a specified collider model.

---

## 3. Meshes and Models (`Mesh.h`, `Model.h`)

### `class Mesh` (`Mesh.h`)
- `Model* CreateModel(float x = 0.0f, float y = 0.0f, float z = 0.0f);`
  Instantiates a new `Model` in the scene at the given world coordinates using this mesh blueprint.
- `void RemoveModel(Model* model);`
  Destroys the specified model instance from the scene.
- `int GetNumTriangles();`
  Returns the total polygon count of the mesh geometry.
- `int GetNumVertices();`
  Returns the total vertex count of the mesh geometry.
- `int GetNumNodes();`
  Returns the number of sub-nodes within the mesh hierarchy.
- `void DisplayNodes();`
  Prints the node tree structure to the console.

### `class Model : virtual public SceneNode` (`Model.h`)
- `Mesh* GetMesh();`
  Returns the `Mesh` pointer from which this model was instantiated.
- `void SetSkin(const string& fileName, int node = -1);`
  Applies a texture bitmap/JPEG file to the model's geometry (or specific sub-node).
- `void EnableLighting(bool useLighting = true);`
  Enables or disables dynamic scene lighting on this model.
- `int GetNumNodes();`
  Returns the number of sub-nodes.
- `SceneNode* GetNode(int node);`
  Returns a pointer to a child sub-node for individual animation/rotation.

---

## 4. Cameras (`Camera.h`)

### Camera Types
```cpp
enum CameraType {
    ManualCamera, // Formerly kManual: stationary by default, supports manual SceneNode movement
    FPSCamera,    // Formerly kFPS: controlled automatically via mouse and cursor keys
    NumCameraTypes
};
```

### `class Camera : virtual public SceneNode` (`Camera.h`)
- `CameraType GetType();`
  Returns `ManualCamera` or `FPSCamera`.
- `void SetNearClip(float nearDistance);`
  Sets near clipping plane distance.
- `void SetFarClip(float farDistance);`
  Sets far clipping plane distance.
- `void SetFOV(float fov);`
  Sets field of view in degrees (typical values: 70–100).
- `void SetMovementSpeed(float speed);`
  Sets translation speed for FPSCamera in units/second.
- `float GetMovementSpeed();`
  Gets translation speed for FPSCamera.
- `void SetRotationSpeed(float speed);`
  Sets rotation speed for FPSCamera in degrees/second.
- `float GetRotationSpeed();`
  Gets rotation speed for FPSCamera.

---

## 5. 2D Sprites (`Sprite.h`)

Sprites are 2D pixel-space bitmaps drawn without mesh geometry.

### `class Sprite` (`Sprite.h`)
- `float GetX();` / `float GetY();`
  Return pixel coordinates from the window top-left origin.
- `void SetX(float x);` / `void SetY(float y);`
  Set pixel coordinates.
- `void SetPosition(float x, float y);`
  Sets 2D pixel coordinates.
- `void SetZ(float z);`
  Sets depth layer (0.0 = front, 1.0 = back) for overlap sorting.
- `void MoveX(float x);` / `void MoveY(float y);`
  Moves sprite in pixel space.
- `void Move(float x, float y);`
  Moves sprite along X and Y axes in pixel space.

---

## 6. Fonts and Text Output (`Font.h`)

### Text Alignment Enums
```cpp
enum HorizontalAlignment {
    LeftAlign,   // Formerly kLeft
    CentreAlign, // Formerly kCentre
    RightAlign   // Formerly kRight
};

enum VerticalAlignment {
    TopAlign,    // Formerly kTop
    MiddleAlign, // Formerly kVCentre
    BottomAlign  // Formerly kBottom
};
```

### Predefined Colour Constants
```cpp
const unsigned int White     = 0xffffffff;
const unsigned int Black     = 0xff000000;
const unsigned int Red       = 0xffff0000;
const unsigned int Green     = 0xff00ff00;
const unsigned int Blue      = 0xff0000ff;
const unsigned int Cyan      = 0xff00ffff;
const unsigned int Magenta   = 0xffff00ff;
const unsigned int Yellow    = 0xffffff00;
const unsigned int LightGrey = 0xffc0c0c0;
const unsigned int MidGrey   = 0xff808080;
const unsigned int Grey      = 0xff808080;
const unsigned int DarkGrey  = 0xff404040;
```

### `class Font` (`Font.h`)
- `void Draw(const string& text, int x, int y, unsigned int colour = Black, HorizontalAlignment xAlign = LeftAlign, VerticalAlignment yAlign = TopAlign);`
  Renders single-frame text at screen pixel coordinates. Must be called per frame inside the game loop to persist.
- `int MeasureTextWidth(const string& text);`
  Returns rendered width of the text string in pixels.
- `int MeasureTextHeight(const string& text);`
  Returns rendered height of the text string in pixels.

---

## 7. Input Handling (`Input.h`, `3DEngine.h`)

### Input Methods in `TLEngine`
- `bool KeyHit(KeyCode keyCode);`
  Returns `true` only on the initial frame a key or mouse button is depressed.
- `bool KeyHeld(KeyCode keyCode);`
  Returns `true` continuously while the key or mouse button remains pressed.
- `bool AnyKeyHit();` / `bool AnyKeyHeld();`
  Report whether any keyboard key or mouse button is depressed.
- `int GetMouseX();` / `int GetMouseY();`
  Return mouse cursor position in pixels relative to window top-left.
- `float GetMouseMovementX();` / `float GetMouseMovementY();`
  Return mouse cursor movement deltas since previous query. Note: returns `float` in TL-Engine 11 header, whereas old documentation stated `int`.
- `void StartMouseCapture();`
  Hides the OS mouse cursor and locks mouse movement input to the game window.
- `void StopMouseCapture();`
  Restores the OS mouse cursor and releases capture.

### Key Code Enumeration (`KeyCode`)
Common key codes:
- Mouse: `Mouse_LButton`, `Mouse_RButton`, `Mouse_MButton`
- Movement: `Key_W`, `Key_A`, `Key_S`, `Key_D`, `Key_Q`, `Key_E`
- Arrows: `Key_Up`, `Key_Down`, `Key_Left`, `Key_Right`
- System/Action: `Key_Space`, `Key_Return`, `Key_Escape`, `Key_Tab`, `Key_Back`
- Alphanumeric: `Key_0` .. `Key_9`, `Key_A` .. `Key_Z`
- Numpad: `Key_Numpad0` .. `Key_Numpad9`

---

## 8. Name Mapping: Old Lab Names vs TL-Engine 11 Names (`LegacyNames.h`)

`LegacyNames.h` provides aliases and `#define` macros for backward compatibility, but modern TL-Engine 11 code should use the updated type names:

| Concept / Identifier | Old Name (Labs / Appendices) | TL-Engine 11 Header Name | Header File | Note / Discrepancy |
| :--- | :--- | :--- | :--- | :--- |
| **Engine Class** | `I3DEngine` | `TLEngine` | `3DEngine.h`, `LegacyNames.h` | `using I3DEngine = TLEngine;` |
| **Mesh Class** | `IMesh` | `Mesh` | `Mesh.h`, `LegacyNames.h` | `using IMesh = Mesh;` |
| **Model Class** | `IModel` | `Model` | `Model.h`, `LegacyNames.h` | `using IModel = Model;` |
| **Scene Node Class** | `ISceneNode` | `SceneNode` | `SceneNode.h`, `LegacyNames.h` | `using ISceneNode = SceneNode;` |
| **Camera Class** | `ICamera` | `Camera` | `Camera.h`, `LegacyNames.h` | `using ICamera = Camera;` |
| **Light Class** | `ILight` | `Light` | `Light.h`, `LegacyNames.h` | `using ILight = Light;` |
| **Sprite Class** | `ISprite` | `Sprite` | `Sprite.h`, `LegacyNames.h` | `using ISprite = Sprite;` |
| **Font Class** | `IFont` | `Font` | `Font.h`, `LegacyNames.h` | `using IFont = Font;` |
| **Engine Type Enum** | `EEngineType` | `EngineType` | `TL-Engine11.h`, `LegacyNames.h` | Macro in LegacyNames.h |
| **Engine Type Variant** | `kTLX` | `TLXStandard` | `TL-Engine11.h`, `LegacyNames.h` | `#define kTLX TLXStandard` |
| **Legacy Engine Variant**| `kTLXLegacy` | `TLXLegacy` | `TL-Engine11.h`, `LegacyNames.h` | May be removed in future |
| **Irrlicht Engine** | `kIrrlicht` | *(Removed)* | `LegacyNames.h` | Unsupported in TL-Engine 11 |
| **Camera Type Enum** | `ECameraType` | `CameraType` | `Camera.h`, `LegacyNames.h` | Macro has stray semicolon in LegacyNames.h |
| **Manual Camera Type** | `kManual` / `kStatic` | `ManualCamera` | `Camera.h`, `LegacyNames.h` | `kStatic` was in old v1.09 doc |
| **FPS Camera Type** | `kFPS` | `FPSCamera` | `Camera.h`, `LegacyNames.h` | Built-in mouse/arrow controls |
| **Horizontal Alignment**| `EHorizAlignment` | `HorizontalAlignment` | `Font.h`, `LegacyNames.h` | Macro in LegacyNames.h |
| **Alignment: Left** | `kLeft` | `LeftAlign` | `Font.h`, `LegacyNames.h` | `#define kLeft LeftAlign` |
| **Alignment: Centre** | `kCentre` | `CentreAlign` | `Font.h`, `LegacyNames.h` | `#define kCentre CentreAlign` |
| **Alignment: Right** | `kRight` | `RightAlign` | `Font.h`, `LegacyNames.h` | `#define kRight RightAlign` |
| **Vertical Alignment** | `EVertAlignment` | `VerticalAlignment` | `Font.h`, `LegacyNames.h` | Macro in LegacyNames.h |
| **Alignment: Top** | `kTop` | `TopAlign` | `Font.h`, `LegacyNames.h` | `#define kTop TopAlign` |
| **Alignment: VCentre** | `kVCentre` | `MiddleAlign` | `Font.h`, `LegacyNames.h` | `#define kVCentre MiddleAlign` |
| **Alignment: Bottom** | `kBottom` | `BottomAlign` | `Font.h`, `LegacyNames.h` | `#define kBottom BottomAlign` |
| **Colour Constants** | `kWhite`, `kRed`, etc. | `White`, `Red`, etc. | `Font.h`, `LegacyNames.h` | `const unsigned int kWhite = White;` |
| **Key State Enum** | `EKeyState` | `KeyState` | `Input.h`, `LegacyNames.h` | Macro in LegacyNames.h |
| **Key Up State** | `kNotPressed` | `KeyUp` | `Input.h`, `LegacyNames.h` | Key currently up |
| **Key Struck State** | `kPressed` | `KeyStruck` | `Input.h`, `LegacyNames.h` | Just pressed on this frame |
| **Key Down State** | `kHeld` | `KeyDown` | `Input.h`, `LegacyNames.h` | Continuously held |
| **Decimal Point Key** | `Key_DecimalPoint` | `Key_Decimal` | `Input.h`, `TL-Engine Appendix 2.pdf.txt` | Appendix 2 has `Key_DecimalPoint`; `Input.h` defines `Key_Decimal = 0x6E` |
| **Max Key Codes** | `kMaxKeyCodes` | `MaxKeyCodes` | `Input.h`, `LegacyNames.h` | Value `0x100` |
| **Mouse Delta Return** | `int` | `float` | `3DEngine.h`, `TL-Engine Appendix 1.pdf.txt` | `GetMouseMovementX/Y` returns float in TL11 |
| **Delta Time Method** | *(None in Appendix 1)* | `FrameTime()` | `3DEngine.h` | Returns seconds elapsed since last frame |
