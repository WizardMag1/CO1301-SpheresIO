/**************************************************************************************************
	Camera.h

	Definition of the ICamera class, defining a camera in the scene
**************************************************************************************************/

#ifndef TL_CAMERA_H_INCLUDED
#define TL_CAMERA_H_INCLUDED

#include "Common.h"    // Common TL-Engine definitions
#include "SceneNode.h" // Base interface class

namespace tle {        // TL-Engine interface


// Types of camera
enum CameraType
{
	ManualCamera,
	FPSCamera,

	NumCameraTypes
};


// Camera class declaration
class Camera : virtual public SceneNode
{
	/*---------------------------------------------------------------------------------------------
		Construction
	---------------------------------------------------------------------------------------------*/
public:
	// No construction needed

	// Destructor
	virtual ~Camera() {}


	/*---------------------------------------------------------------------------------------------
		Properties
	---------------------------------------------------------------------------------------------*/
public:
	// Returns the camera type
	virtual CameraType GetType() = 0;


	/*---------------------------------------------------------------------------------------------
		Projection
	---------------------------------------------------------------------------------------------*/
public:
	// Sets the near clipping distance - models closer than this distance will be clipped.
	// May need to use this if the scale of the scene is very large or small
	virtual void SetNearClip(float nearDistance) = 0;

	// Sets the far clipping distance - models further than this distance will be clipped.
	// May need to use this if the scale of the scene is very large or small
	virtual void SetFarClip(float farDistance) = 0;

	// Sets the field of view (FOV). This is the angle from the left to the right of the viewport
	// Higher values give a "fish-eye" effect, lower values a "telephoto" effect
	// Values can range roughly from 20 to 150 (degrees). Default for a game is around 70-100
	virtual void SetFOV(float fov) = 0;


	/*---------------------------------------------------------------------------------------------
		Camera Speed (FPS camera)
	---------------------------------------------------------------------------------------------*/
public:
	// Set the movement speed of an FPS camera, in units per second. No effect for other camera types
	virtual void SetMovementSpeed(float speed) = 0;

	// Get the movement speed of an FPS camera, in units per second. Returns 0 for other camera types
	virtual float GetMovementSpeed() = 0;

	// Set the rotation speed of an FPS camera, in degrees per second. No effect for other camera types
	virtual void SetRotationSpeed(float speed) = 0;

	// Get the movement speed of an FPS camera, in degrees per second. Returns 0 for other camera types
	virtual float GetRotationSpeed() = 0;

}; // class Camera


} // namespace tle


#endif // TL_CAMERA_H
