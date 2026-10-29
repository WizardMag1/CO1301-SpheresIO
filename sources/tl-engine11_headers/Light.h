/**************************************************************************************************
	Light.h

	Definition of the Light class, a definition of a light in the scene
**************************************************************************************************/

#ifndef TL_LIGHT_H_INCLUDED
#define TL_LIGHT_H_INCLUDED

#include "Common.h"    // Common TL-Engine definitions
#include "SceneNode.h" // Base interface class

namespace tle          // TL-Engine interface
{


// Types for Lights
enum LightType
{
	DirectionalLight,
	PointLight,
	SpotLight,

	NumLightTypes
};


// Light class declaration
class Light : virtual public SceneNode
{
	/*---------------------------------------------------------------------------------------------
		Construction
	---------------------------------------------------------------------------------------------*/
public:
	// No construction needed

	// Destructor
	virtual ~Light() {}


	/*---------------------------------------------------------------------------------------------
		Properties
	---------------------------------------------------------------------------------------------*/
public:
	// Returns the light type
	virtual LightType GetType() = 0;

	// Enable / disable the light
	virtual void Enable(bool enable = true) = 0;

	// Set the light colour
	virtual void SetColour(float red, float green, float blue) = 0;

	// Set the light radius
	virtual void SetRadius(float radius) = 0;

}; // class Light


} // namespace tle

#endif // TL_LIGHT_H_INCLUDED
