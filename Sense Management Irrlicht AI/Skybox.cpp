#include "Skybox.h"
#include "GameObjectFactory.h"

const char* Skybox::s_type = "Skybox";

static bool a = TheGameObjectFactory::Instance()->AddType("Skybox", Skybox::CreateSkybox);

GameObject* Skybox::CreateSkybox()
{
	return new Skybox;
}

bool Skybox::Load(File* pFile) // TODO - texture manager
{
	std::string top;
	std::string bottom;
	std::string left;
	std::string right;
	std::string back;
	std::string front;

	pFile->GetString(&top);
	pFile->GetString(&bottom);
	pFile->GetString(&left);
	pFile->GetString(&right);
	pFile->GetString(&back);
	pFile->GetString(&front);

	TheIrrlichtObject::Instance()->GetSceneManager()->addSkyBoxSceneNode(
		TheIrrlichtObject::Instance()->GetVideoDriver()->getTexture(top.c_str()), 
		TheIrrlichtObject::Instance()->GetVideoDriver()->getTexture(bottom.c_str()),
		TheIrrlichtObject::Instance()->GetVideoDriver()->getTexture(left.c_str()),
		TheIrrlichtObject::Instance()->GetVideoDriver()->getTexture(right.c_str()),
		TheIrrlichtObject::Instance()->GetVideoDriver()->getTexture(front.c_str()),
		TheIrrlichtObject::Instance()->GetVideoDriver()->getTexture(back.c_str()));

	return true;
}

void Skybox::Draw()
{
}

void Skybox::Update(float dt)
{
}

void Skybox::Clean()
{
}

void Skybox::Reset()
{
}