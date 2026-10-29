/**************************************************************************************************
	Sprite.h

	Definition of the ISprite class, a class to manipulate a 2D sprite
**************************************************************************************************/

#ifndef TL_SPRITE_H
#define TL_SPRITE_H

#include "Common.h" // Common TL-Engine definitions

namespace tle       // TL-Engine interface
{


// Sprite class declaration
class Sprite
{
	/*---------------------------------------------------------------------------------------------
		Construction
	---------------------------------------------------------------------------------------------*/
public:
	// No construction needed

	// Destructor
	virtual ~Sprite() {}


	/*---------------------------------------------------------------------------------------------
		Positioning
	---------------------------------------------------------------------------------------------*/
public:
	// Get current X position (in pixels, from 0,0 at top-left of window)
	virtual float GetX() = 0;

	// Get current Y position (in pixels, from 0,0 at top-left of window)
	virtual float GetY() = 0;

	// Set current X position (in pixels, from 0,0 at top-left of window)
	virtual void SetX(float x) = 0;

	// Set current Y position (in pixels, from 0,0 at top-left of window)
	virtual void SetY(float y) = 0;

	// Set current Z depth (from 0.0 to 1.0, sprites with smaller values are drawn in front)
	virtual void SetZ(float z) = 0;

	// Set current position X,Y (in pixels, from 0,0 at top-left of window)
	virtual void SetPosition(float x, float y) = 0;


	/*---------------------------------------------------------------------------------------------
		Movement
	---------------------------------------------------------------------------------------------*/
public:
	// Move sprite in X direction by given number of pixels
	virtual void MoveX(float x) = 0;

	// Move sprite in X direction by given number of pixels
	virtual void MoveY(float y) = 0;

	// Move sprite in X and Y by given number of pixels
	virtual void Move(float x, float y ) = 0;

	// TODO Z movement - scaling; rotation

}; // class Sprite


} // namespace tle


#endif // TL_SPRITE_H
