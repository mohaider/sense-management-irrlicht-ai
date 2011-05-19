#ifndef IRRLICHT_MESH_H
#define IRRLICHT_MESH_H

/*
	IrrlichtMesh.h
	Shaun Mitchell
*/

#include <string>
#include "IrrlichtObject.h"

class IrrlichtMesh
{
public:

	IrrlichtMesh();
	~IrrlichtMesh();

	// load and animated mesh
	virtual void Load(const char* meshFilename, const char* textureFilename, float x, float y, float z);

	IAnimatedMeshSceneNode* GetSceneNode() { return m_pMeshSceneNode; }
private:

	const char* m_meshFile;
	const char* m_textureFile;
	float x;
	float y;
	float z;

	IAnimatedMesh* m_pMesh;
	IAnimatedMeshSceneNode* m_pMeshSceneNode;
	ITriangleSelector* m_pTriSelector;
};

#endif