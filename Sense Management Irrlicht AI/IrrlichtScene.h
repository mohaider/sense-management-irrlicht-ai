#ifndef IRRLICHT_SCENE_H
#define IRRLICHT_SCENE_H

#include "IrrlichtObject.h"
#include <vector>

class IrrlichtScene
{
public:

	bool Load(const char* filename);

	array<ISceneNode*>& GetSceneNodes() { return m_sceneNodes; }

private:

	array<ISceneNode*> m_sceneNodes;
};

#endif