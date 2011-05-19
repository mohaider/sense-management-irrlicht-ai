#include "IrrlichtScene.h"
#include "Game.h"

bool IrrlichtScene::Load(const char* filename)
{
	TheIrrlichtObject::Instance()->GetSceneManager()->loadScene(filename);

	TheIrrlichtObject::Instance()->GetSceneManager()->getSceneNodesFromType(ESNT_ANY, m_sceneNodes);

	for(u32 i = 0; i < m_sceneNodes.size(); ++i)
	{
		ISceneNode* node = m_sceneNodes[i];
		ITriangleSelector* selector = 0;

		switch(node->getType())
		{
		case scene::ESNT_CUBE:
			break;

		case scene::ESNT_MESH:
			TheGame::Instance()->GetBoundingBoxes().push_back(node);
			break;

		case scene::ESNT_SPHERE: // spheres are used as way points from irr edit, oh yes! I'm clever! :P
			node->setVisible(false);
			TheGame::Instance()->GetPatrolPath().push_back(vector2df(node->getPosition().X, node->getPosition().Z));
			break;

		case scene::ESNT_TERRAIN:
			selector = TheIrrlichtObject::Instance()->GetSceneManager()->createTerrainTriangleSelector((scene::ITerrainSceneNode*)node);
			break;

		case scene::ESNT_OCTREE:
			break;

		case scene::ESNT_LIGHT:
			TheGame::Instance()->GetLights().push_back((ILightSceneNode*)node);
			break;

		default:
			// Don't create a selector for this node type
			break;
		}
	}
	return true;
}