#ifndef SKYBOX_H
#define SKYBOX_H

#include "GameObject.h"
#include "IrrlichtObject.h"

class Skybox : public GameObject
{
public:

	Skybox() {}
	virtual ~Skybox() {}

	static const char* s_type;
	static GameObject* CreateSkybox();

	virtual const char* GetTypeName() const { return "Skybox"; }

	virtual bool Load(File* pFile);
	virtual void Draw();
	virtual void Update(float dt);
	virtual void Clean();
	virtual void Reset();
};

#endif