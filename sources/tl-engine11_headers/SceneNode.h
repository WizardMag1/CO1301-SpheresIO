/**************************************************************************************************
	SceneNode.h

	Definition of the SceneNode class, a class defining the basic interface
	such as positioning and rotation for all kinds of nodes in the scene graph
**************************************************************************************************/

#ifndef TL_SCENE_NODE_H_INCLUDED
#define TL_SCENE_NODE_H_INCLUDED

#include "Common.h" // Common TL-Engine definitions

namespace tle {     // TL-Engine interface


// Forward declarations
class Model;


// SceneNode class declaration
class SceneNode
{
	/*---------------------------------------------------------------------------------------------
		Construction
	---------------------------------------------------------------------------------------------*/

	// No construction needed

protected:
	// Protected destructor - must use Remove* functions to destroy scene nodes
	virtual ~SceneNode() {}


	/*---------------------------------------------------------------------------------------------
		Position
	---------------------------------------------------------------------------------------------*/
public:
	// Get current world X position
	virtual float GetX() = 0;

	// Get current world Y position
	virtual float GetY() = 0;

	// Get current world Z position
	virtual float GetZ() = 0;

	// Set the world X position
	virtual void SetX(float x) = 0;

	// Set the world Y position
	virtual void SetY(float y) = 0;

	// Set the world Z position
	virtual void SetZ(float z) = 0;

	// Set the world X, Y & Z positions
	virtual void SetPosition(float x, float y, float z) = 0;


	// Get current local X position (relative to parent)
	virtual float GetLocalX() = 0;

	// Get current local Y position (relative to parent)
	virtual float GetLocalY() = 0;

	// Get current local Z position (relative to parent)
	virtual float GetLocalZ() = 0;

	// Set the local X position (relative to parent)
	virtual void SetLocalX(float x) = 0;

	// Set the local Y position (relative to parent)
	virtual void SetLocalY(float y) = 0;

	// Set the local Z position (relative to parent)
	virtual void SetLocalZ(float z) = 0;

	// Set the local X, Y & Z positions (relative to parent)
	virtual void SetLocalPosition(float x, float y, float z) = 0;


	/*---------------------------------------------------------------------------------------------
		Movement
	---------------------------------------------------------------------------------------------*/
public:
	// Move in the world X direction by the given amount
	virtual void MoveX(float x) = 0;

	// Move in the world Y direction by the given amount
	virtual void MoveY(float y) = 0;

	// Move in the world Z direction by the given amount
	virtual void MoveZ(float z) = 0;

	// Move in the world X, Y & Z directions by the given amounts
	virtual void Move(float x, float y, float z) = 0;


	// Move in the local X direction by the given amount
	virtual void MoveLocalX(float x) = 0;

	// Move in the local Y direction by the given amount
	virtual void MoveLocalY(float y) = 0;

	// Move in the local Z direction by the given amount
	virtual void MoveLocalZ(float z) = 0;

	// Move in the local X, Y & Z directions by the given amounts
	virtual void MoveLocal(float x, float y, float z) = 0;


	/*---------------------------------------------------------------------------------------------
		Orientation
	---------------------------------------------------------------------------------------------*/
public:
	// Reset the orientation (rotations) to the default
	virtual void ResetOrientation() = 0;

	// Set orientation to look at a given scene node
	virtual void LookAt(SceneNode* target) = 0;

	// Set orientation to look at a given world (X, Y, Z) position
	virtual void LookAt(float x, float y, float z) = 0;


	/*---------------------------------------------------------------------------------------------
		Rotation
	---------------------------------------------------------------------------------------------*/
public:
	// Rotate around the world X axis by the given amount (in degrees)
	virtual void RotateX(float x) = 0;

	// Rotate around the world Y axis by the given amount (in degrees)
	virtual void RotateY(float y) = 0;

	// Rotate around the world Z axis by the given amount (in degrees)
	virtual void RotateZ(float z) = 0;

	// Rotate around the local X axis by the given amount (in degrees)
	virtual void RotateLocalX(float x) = 0;

	// Rotate around the local Y axis by the given amount (in degrees)
	virtual void RotateLocalY(float y) = 0;

	// Rotate around the local Z axis by the given amount (in degrees)
	virtual void RotateLocalZ(float z) = 0;


	/*---------------------------------------------------------------------------------------------
		Scaling
	---------------------------------------------------------------------------------------------*/
public:
	// Scale by the given amount
	virtual void Scale(float scale) = 0;

	// Scale in the X dimension by the given amount
	virtual void ScaleX(float x) = 0;

	// Scale in the Y dimension by the given amount
	virtual void ScaleY(float x) = 0;

	// Scale in the Z dimension by the given amount
	virtual void ScaleZ(float x) = 0;

	// Reset scale to the default
	virtual void ResetScale() = 0;


	/*---------------------------------------------------------------------------------------------
		Matrix Access
	---------------------------------------------------------------------------------------------*/
public:
	// Get the transformation matrix of this node. If the node has a parent, this matrix will be
	// relative to the parent. Otherwise it will be in world space. Will store 16 floats defining the
	// matrix into the given pointer (can pass a pointer to a 4x4 array of floats)
	virtual void GetMatrix(float* matrixElts) = 0;

	// Set the transformation matrix of this node. If the node has a parent, this matrix must be
	// relative to the parent. Otherwise it must be in world space. Will get 16 floats defining the
	// matrix from the given pointer (can pass a pointer to a 4x4 array of floats)
	virtual void SetMatrix(float* matrixElts) = 0;


	/*---------------------------------------------------------------------------------------------
		Heirarchy
	---------------------------------------------------------------------------------------------*/
public:
	// Attaches this object to another object (its parent). All positioning for this object will
	// become relative to the parent and the parent's movement/rotation will affect this object.
	// An object can only be attached to one parent. Some nodes can't be parented & return false
	virtual bool AttachToParent(SceneNode* parent) = 0;

	// Detaches this object from any parent it has. Some nodes can't be parented & return false
	virtual bool DetachFromParent() = 0;


	/*---------------------------------------------------------------------------------------------
		Miscellaneous
	---------------------------------------------------------------------------------------------*/
public:
	// Enable collision detection between this object and the given model.
	// Can optionally supply a gravity setting for this object (use 0 to switch gravity off), and
	// the radiuses of the bounding volume for this object. Not all models support can support
	// collision - if not this function will return false.
	virtual bool EnableCollisions(Model* collider, float gravity = -9.8f,
		                          float radiusX = 0.0f, float radiusY = 0.0f, float radiusZ = 0.0f) = 0;

}; // class SceneNode


} // namespace tle

#endif // TL_SCENE_NODE_H_INCLUDED
