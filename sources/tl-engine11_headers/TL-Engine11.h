/**************************************************************************************************
	TL-Engine11.h

	Interface for the TL-Engine11
**************************************************************************************************/

#ifndef TL_ENGINE_11_H_INCLUDED
#define TL_ENGINE_11_H_INCLUDED

#include "3DEngine.h" // 3D engine interface

namespace tle {       // TL-Engine interface


/*-------------------------------------------------------------------------
	TL-Engine start-up declarations 
-------------------------------------------------------------------------*/

// Enumeration of supported 3D engines
enum EngineType
{
	TL11,        // New engine
	TLXStandard, // Old engine
	TLXLegacy,   // Legacy variant of old engine - may be removed

	NumEngineTypes
};

// Factory for TLEngine derived classes
TLEngine* New3DEngine(EngineType engineType);


} // namespace tle


// Defines old versions of names (e.g. I3DEngine instead of TLEngine, or IMesh instead of Mesh), so old code will compile.
// Must include this after everything else is defined, so unusually put here at the end of the file
#include "LegacyNames.h" 


#endif // TL_ENGINE_11_H_INCLUDED