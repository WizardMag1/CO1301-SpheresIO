/**************************************************************************************************
	3DEngine.h

	Definition of the TLEngine class, defining common interface for all
	engine implementations
**************************************************************************************************/

#ifndef TL_3D_ENGINE_H_INCLUDED
#define TL_3D_ENGINE_H_INCLUDED

#include <string>    // Standard libraries
using std::string;

#include "Mesh.h"   // Mesh interface
#include "Model.h"  // Model interface
#include "Light.h"  // Light interface
#include "Camera.h" // Camera interface
#include "Sprite.h" // Sprite interface
#include "Font.h"   // Font interface
#include "Input.h"  // Input interface
#include "Common.h" // Common TL-Engine definitions

namespace tle {     // TL-Engine interface


class TLEngine
{
	/*---------------------------------------------------------------------------------------------
		Construction
	---------------------------------------------------------------------------------------------*/
public:
	// No construction needed

	// Destructor
	virtual ~TLEngine() {}


	/*---------------------------------------------------------------------------------------------
		Main control
	---------------------------------------------------------------------------------------------*/
public:
	// Starts the 3D engine fullscreen. This can only be done once after creating a 3D engine.
	// Can pass desired width and height but the engine can override these. Returns false on failure
	virtual bool StartFullscreen(int width = 1920, int height = 1280) = 0;

	// Starts the 3D engine in a window of the given dimensions. This can only
	// be done once after creating a 3D engine.
	virtual void StartWindowed(int width = 1600, int height = 900) = 0;


	// Delete the 3D engine. Must call this before finishing the program
	// to correctly clean up the memory used by the 3D engine.
	virtual void Delete() = 0;


	// Request that the 3D engine stops - will ensure IsRunning returns
	// false. Use this function to terminate the main loop of your
	// application from other parts of the code.
	virtual void Stop() = 0;


	/*---------------------------------------------------------------------------------------------
		Properties
	---------------------------------------------------------------------------------------------*/
public:
	// Returns true if the 3D engine is initialised and running. Will 
	// return false if the system or user has requested the engine to stop.
	// Use this function to control the main loop of your application.
	virtual bool IsRunning() = 0;

	// Returns true if the engine window/fullscreen is active, i.e. it is the
	// foreground window that the user is working with. If this function
	// returns false, then any keyboard/mouse input should be ignored - it is
	// intended for another window.
	virtual bool IsActive() = 0;


	// Return a handle to the engine window
	virtual int GetWindow()  = 0;

	// Return the width of the engine window/fullscreen.
	virtual int GetWidth() = 0;

	// Return the height of the engine window/fullscreen.
	virtual int GetHeight() = 0;


	/*---------------------------------------------------------------------------------------------
		Media folders
	---------------------------------------------------------------------------------------------*/
public:
	// Add a folder to the list of folders searched for media
	virtual void AddMediaFolder(const string& folderName) = 0;

	// Remove a folder from the list of folders searched for media, returns
	// true on success (folder was found and removed)
	virtual bool RemoveMediaFolder(const string& folderName) = 0;

	// Clears the list of folders searched for media
	virtual void ClearMediaFolders() = 0;


	/*---------------------------------------------------------------------------------------------
		Meshes
	---------------------------------------------------------------------------------------------*/
public:
	// Loads a 3D mesh from a file, returns a pointer to the mesh loaded or nullptr for an error.
	// Only loads the mesh data, does not actually create a model in the scene. Use the mesh
	// function CreateModel to create models to place in the scene. Note that different 3D engines
	// support different file types.
	virtual Mesh* LoadMesh(const string& meshFileName) = 0;

	// Removes a mesh from the system, also removes any models in the scene that use the mesh.
	virtual void RemoveMesh(Mesh* mesh) = 0;


	/*---------------------------------------------------------------------------------------------
		Cameras
	---------------------------------------------------------------------------------------------*/
public:
	// Creates a camera of a given type (ManualCamera [default] or FPSCamera),
	// positioned at a given location. Returns a pointer to the new camera.
	virtual Camera* CreateCamera(CameraType cameraType = ManualCamera,
	                             float x = 0.0f, float y = 12.0f, float z = -30.0f) = 0;

	// Removes a camera from the scene.
	virtual void RemoveCamera(Camera* camera) = 0;


	/*---------------------------------------------------------------------------------------------
		Lights
	---------------------------------------------------------------------------------------------*/
public:
	// Set the ambient light colour for the scene. The ambient light colour defaults
	// to white (i.e. everything is fully lit) - this function overrides that setting
	virtual void SetAmbientLight(float red = 1.0f, float green = 1.0f, float blue = 1.0f) = 0;

	// Creates a point light positioned at a given location and with the given colour (red,
	// green and blue in range 0.0f to 1.0f). Returns a pointer to the new light or nullptr on error
	virtual Light* CreatePointLight(float x = 0.0f, float y = 0.0f, float z = 0.0f,
	                                float red = 1.0f, float green = 1.0f, float blue = 1.0f,
	                                float radius = 100.0f) = 0;

	// Removes a light from the scene.
	virtual void RemoveLight(Light* light) = 0;


	/*---------------------------------------------------------------------------------------------
		Sprites
	---------------------------------------------------------------------------------------------*/
public:
	// Create a sprite in the scene using the given image file. 
	// Returns a pointer to the sprite loaded or nullptr for an error.
	// May optionally set the initial position for the sprite
	virtual Sprite* CreateSprite(const string& spriteName, float x = 0.0f, float y = 0.0f, float z = 0.0f) = 0;

	// Removes a sprite from the scene
	virtual void RemoveSprite(Sprite* sprite) = 0;


	/*---------------------------------------------------------------------------------------------
		Rendering
	---------------------------------------------------------------------------------------------*/
public:
	// Draw everything in the scene from the viewpoint of the given camera.
	// If no camera is supplied, the most recently created camera is used.
	virtual void DrawScene(Camera* camera = nullptr) = 0;

	// Get time between last two calls to DrawScene in seconds. Returns 1/60 if there haven't been two calls yet.
	// Note: Current FPS = 1 / frame time
	virtual float FrameTime() = 0;

	// Get average FPS over the last second - use to display FPS
	virtual int AverageFPS() = 0;

	// Lock FPS to monitor refresh rate (e.g. 60fps, 120fps, 240fps)
	virtual void EnableVsync(bool enable = true) = 0;


	/*---------------------------------------------------------------------------------------------
		Fonts
	---------------------------------------------------------------------------------------------*/
public:
	// Returns a pointer to the default (built-in) font
	virtual Font* DefaultFont() = 0;

	// Loads a font from a file, returns a pointer to the font loaded or nullptr for an error
	// May optionally pass point size of font
	virtual Font* LoadFont(const string& fontName, unsigned int fontSize = 24) = 0;

	// Removes a font from the engine
	virtual void RemoveFont(Font* font) = 0;


	/*---------------------------------------------------------------------------------------------
		User Input
	---------------------------------------------------------------------------------------------*/
public:
	// Returns true when a given key or button is first pressed down. Use
	// for one-off actions or toggles. Example key codes: Key_A or
	// Mouse_LButton, see Input.h for a full list.
	virtual bool KeyHit(KeyCode keyCode) = 0;

	// Returns true as long as a given key or button is held down. Use for
    // continuous action or motion. Example key codes: Key_A or
	// Mouse_LButton, see Input.h for a full list.
	virtual bool KeyHeld(KeyCode keyCode) = 0;

	// Returns true when any key or button is first pressed down
	virtual bool AnyKeyHit() = 0;

	// Returns true when any key or button is being held down
	virtual bool AnyKeyHeld() = 0;


    // Returns the X position of the mouse (relative to the top-left of the
	// 3D engine window/fullscreen).
	virtual int GetMouseX() = 0;

    // Returns the Y position of the mouse (relative to the top-left of the
	// 3D engine window/fullscreen).
	virtual int GetMouseY() = 0;

	// Returns the wheel position of the mouse
	virtual float GetMouseWheel() = 0;


	// Returns the X mouse movement since the last call to this function
	virtual float GetMouseMovementX() = 0;

	// Returns the Y mouse movement since the last call to this function
	virtual float GetMouseMovementY() = 0;

	// Returns the mouse wheel movement since the last call to this function
	virtual float GetMouseWheelMovement() = 0;

	// Set mouse smoothing - a float from 0.0 to 1.0 or so. Default is 0.3
	// Affects GetMouseMovementX and GetMouseMovementY
	virtual void SetMouseSmoothing(float smoothing) = 0;


	// Start capturing the mouse. The mouse cursor will be removed and all
	// mouse movement captured by the 3D engine window. Allows direct mouse 
	// control for games that do not need a mouse cursor.
	virtual void StartMouseCapture() = 0;

	// Stops capturing the mouse. See 'StartMouseCapture'.
	virtual void StopMouseCapture() = 0;


	/*---------------------------------------------------------------------------------------------
		Miscellaneous
	---------------------------------------------------------------------------------------------*/
public:
	// Changes the 3D engine window title - no effect in fullscreen mode
	virtual void SetWindowCaption(const string& text) = 0;

	// Time since engine was started in seconds
	virtual float Timer() = 0;


}; // class I3DEngine


} // namespace tle

#endif // TL_3D_ENGINE_H_INCLUDED
