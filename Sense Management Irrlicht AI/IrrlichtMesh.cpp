#include "IrrlichtMesh.h"
#include "Game.h"

IrrlichtMesh::IrrlichtMesh()
{
}

IrrlichtMesh::~IrrlichtMesh()
{
}

void IrrlichtMesh::Load(const char* meshFilename, const char* texFilename, 
float x, float y, float z)
{
    
	m_pMesh = TheIrrlichtObject::Instance()->GetSceneManager()->getMesh(meshFilename);
	
	m_pMeshSceneNode = TheIrrlichtObject::Instance()->GetSceneManager()->addAnimatedMeshSceneNode(m_pMesh);
	
	m_pMeshSceneNode->setPosition(vector3df(x,y,z));
	m_pMeshSceneNode->setScale(vector3df(3,3,3));
	
	m_pMeshSceneNode->setMaterialTexture(0, TheIrrlichtObject::Instance()->GetVideoDriver()->getTexture(texFilename));
	
	m_pMeshSceneNode->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	m_pMeshSceneNode->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

	m_pMeshSceneNode->setAnimationSpeed(20.0f);

	m_pMeshSceneNode->setFrameLoop(206, 250);
}

