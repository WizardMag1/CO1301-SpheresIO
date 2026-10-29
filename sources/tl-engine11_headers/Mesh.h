/**************************************************************************************************
	Mesh.h
	
	Definition of the Mesh class, defining geometry to render
**************************************************************************************************/

#ifndef TL_MESH_H_INCLUDED
#define TL_MESH_H_INCLUDED

#include "Common.h" // Common TL-Engine definitions

namespace tle {     // TL-Engine interface


// Forward declarations
class Model;


// Mesh class declaration
class Mesh
{
	/*---------------------------------------------------------------------------------------------
		Construction
	---------------------------------------------------------------------------------------------*/
public:
	// No construction needed

	// Destructor
	virtual ~Mesh() {}


	/*---------------------------------------------------------------------------------------------
		Model Creation
	---------------------------------------------------------------------------------------------*/
public:
	// Uses this mesh to create a model in the scene at a given position.
	// Returns a pointer to the new model or nullptr for an error.
	virtual Model* CreateModel(float x = 0.0f, float y = 0.0f, float z = 0.0f) = 0;

	// Removes one of this mesh's models from the scene
	virtual void RemoveModel(Model* model) = 0;


	/*---------------------------------------------------------------------------------------------
		Properties
	---------------------------------------------------------------------------------------------*/
public:
	// Return total number of triangles in the mesh
	virtual int GetNumTriangles() = 0;

	// Return total number of vertices in the mesh
	virtual int GetNumVertices() = 0;

	// Get number of sub-nodes in the mesh
	virtual int GetNumNodes() = 0;

	// Output node hierarchy to console window. Displays node numbers, names and tree structure
	virtual void DisplayNodes() = 0;



	/*---------------------------------------------------------------------------------------------
		Triangle Enumeration
	---------------------------------------------------------------------------------------------*/
public:
	// Request an enumeration of the triangles in the mesh. Get each individual triangle, one-by-one
	// with calls to GetTriangle. Finish the enumeration with EndEnumTriangles
	virtual void BeginEnumTriangles() = 0;

	// Get the next triangle in the mesh, used after BeginEnumTriangles. Fills the supplied pointers
	// with three (x,y,z) float triplets. Returns true if a triangle was successfully returned, false
	// if there are no more triangles to enumerate
	virtual bool GetTriangle(float* v1, float* v2, float* v3) = 0;

	// Finish enumerating triangles in the mesh, used after BeginEnumTriangles
	virtual void EndEnumTriangles() = 0;


	/*---------------------------------------------------------------------------------------------
		Vertex Enumeration
	---------------------------------------------------------------------------------------------*/
public:
	// Request an enumeration of the vertices in the mesh. Get each individual vertex, one-by-one
	// with calls to GetVertex. Finish the enumeration with EndEnumVertices
	virtual void BeginEnumVertices() = 0;

	// Get the next vertex in the mesh, used after BeginEnumVertices. Fills the supplied pointer
	// with a three (x,y,z) float triplet. Returns true if a vertex was successfully returned, false
	// if there are no more vertices to enumerate
	virtual bool GetVertex(float* v) = 0;

	// Finish enumerating vertices in the mesh, used after BeginEnumVertices
	virtual void EndEnumVertices() = 0;

}; // class Mesh


} // namespace tle

#endif // TL_MESH_H_INCLUDED
