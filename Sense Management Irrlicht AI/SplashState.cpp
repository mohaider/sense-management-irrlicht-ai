
#include <stdio.h>

#include "Game.h"
#include "SplashState.h"
#include "PlayState.h"
#include "EventHandler.h"
#include <assert.h>
#include <iostream>

SplashState SplashState::m_SplashState;

ITexture* image = NULL;

bool SplashState::Init()
{
	TheIrrlichtObject::Instance()->GetVideoDriver()->getMaterial2D().TextureLayer[0].BilinearFilter=true;
	TheIrrlichtObject::Instance()->GetVideoDriver()->getMaterial2D().AntiAliasing=video::EAAM_QUALITY;

	image = TheIrrlichtObject::Instance()->GetVideoDriver()->getTexture("assets/textures/intro.png");
	return true;
}

// Free textures and memory
void SplashState::Clean()
{
}

// Pause this state
void SplashState::Pause()
{

}

// Resume this state
void SplashState::Resume()
{
}

// Handle events // player input and exit
void SplashState::HandleEvents()
{
	if(TheEventReceiver::Instance()->IsKeyDown(KEY_ESCAPE))
	{
		exit(0);
	}

	if(TheEventReceiver::Instance()->IsKeyDown(KEY_RETURN))
	{
		TheGame::Instance()->ChangeState(PlayState::Instance());
	}
}

// Update the state // and player
void SplashState::Update(float dt) 
{
}

// Render the state // this is done through the scene graph of irrlicht
void SplashState::Draw() 
{
	TheIrrlichtObject::Instance()->GetVideoDriver()->beginScene(true, true, SColor(255, 0, 0, 0));
	
	TheIrrlichtObject::Instance()->GetVideoDriver()->enableMaterial2D();
	TheIrrlichtObject::Instance()->GetVideoDriver()->draw2DImage(image, core::rect<s32>(0,0,800,600),
		core::rect<s32>(0,0,800,600));
	TheIrrlichtObject::Instance()->GetVideoDriver()->enableMaterial2D(false);

	TheIrrlichtObject::Instance()->GetVideoDriver()->endScene();
}
