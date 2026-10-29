/**************************************************************************************************
	Model.h

	Definition of the Model class, defining an instance of a mesh in the scene
**************************************************************************************************/

#ifndef TL_MODEL_H_INCLUDED
#define TL_MODEL_H_INCLUDED

#include <string>      // Standard libraries
using std::string;

#include "Common.h"    // Common TL-Engine definitions
#include "SceneNode.h" // Base interface class

namespace tle {        // TL-Engine interface


// Forward declarations
class Mesh;


// Model class declaration
class Model : virtual public SceneNode
{
	/*---------------------------------------------------------------------------------------------
		Construction
	---------------------------------------------------------------------------------------------*/
public:
	// No construction needed

	// Destructor
	virtual ~Model() {}


	/*---------------------------------------------------------------------------------------------
		Properties
	---------------------------------------------------------------------------------------------*/
public:
	virtual Mesh* GetMesh() = 0;


	/*---------------------------------------------------------------------------------------------
		Nodes
	---------------------------------------------------------------------------------------------*/
public:
	// Get number of sub-nodes for this model
	virtual int GetNumNodes() = 0;

	// Get a sub-node of this model, which can then be manipulated seperately
	// Model nodes are removed when their models are. Returns nullptr for invalid node
	virtual SceneNode* GetNode(int node) = 0;


	/*---------------------------------------------------------------------------------------------
		Rendering Settings
	---------------------------------------------------------------------------------------------*/
public:
	// Enable / disable lighting on this model (disabled for newly created models)
	virtual void EnableLighting(bool useLighting = true) = 0;

	// Sets the skin used to texture part of a model, by default will affect first node that has
	// any geometry, but can optionally ask to affect a different node
	virtual void SetSkin(const string& fileName, int node = -1) = 0;

}; // class Model


} // namespace tle

#endif // TL_MODEL_H_INCLUDED
