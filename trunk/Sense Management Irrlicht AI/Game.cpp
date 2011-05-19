#include "Game.h"
#include "GameState.h"
#include "SoundManager.h"
#include "Player.h"
#include <assert.h>
#include <stdio.h>
#include <sstream>

void Game::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	// load the irr edit scene
	m_pIrrScene = new IrrlichtScene();
	m_pIrrScene->Load("assets/newscene.irr");

	m_camera = TheIrrlichtObject::Instance()->GetSceneManager()->addCameraSceneNode(0, vector3df(0.0f,0.0f,0.0f), vector3df(0.0f,0.0f,0.0f), -1);

	m_cameraDirection = 0;
	m_zdirection = 0;
}

void Game::Clean()
{
	while(!states.empty())
	{
		states.back()->Clean();
		states.pop_back();
	}
}

void Game::ChangeState(GameState* state)
{
	if(!states.empty())
	{
		states.back()->Clean();
		states.pop_back();
	}

	states.push_back(state);
	states.back()->Init();
}

void Game::PushState(GameState* state)
{
	if(!states.empty())
	{
		states.back()->Pause();
	}

	states.push_back(state);
	states.back()->Init();
}

void Game::PopState()
{
	if(!states.empty())
	{
		states.back()->Clean();
		states.pop_back();
	}

	if(!states.empty())
	{
		states.back()->Resume();
	}
}

void Game::Draw()
{
	states.back()->Draw();
}

void Game::HandleEvents()
{
	states.back()->HandleEvents();
}

void Game::Update(float dt)
{
	states.back()->Update(dt);
	SoundManager::Instance()->Update();
}

void Game::MoveCameraControl()
{
	// Third person camera took ages to implement!
	core::position2d<f32> cursorPos = TheIrrlichtObject::Instance()->GetIrrlichtDevice()->getCursorControl()->getRelativePosition(); 
	scene::ICameraSceneNode* camera = TheIrrlichtObject::Instance()->GetSceneManager()->getActiveCamera(); 
	core::vector3df cameraPos = m_camera->getAbsolutePosition(); 

	float change_x = ( cursorPos.X - 0.5f ) * 256.0f; 
	float change_y = ( cursorPos.Y - 0.5f ) * 256.0f; 

	m_cameraDirection += change_x; 
	m_zdirection -= change_y; 
	if( m_zdirection <- 90 ) 
		m_zdirection = -90; 
	else 
		if( m_zdirection > 90 ) 
			m_zdirection = 90; 

	TheIrrlichtObject::Instance()->GetIrrlichtDevice()->getCursorControl()->setPosition( 0.5f, 0.5f ); 

	core::vector3df playerPos = ThePlayer::Instance()->GetMesh()->GetSceneNode()->getPosition();

	float xf = playerPos.X - cos( m_cameraDirection * PI / 180.0f ) * 64.0f; 
	float yf = playerPos.Y - sin( m_zdirection * PI / 180.0f ) * 64.0f; 
	float zf = playerPos.Z + sin( m_cameraDirection * PI / 180.0f ) * 64.0f; 

	m_camera->setPosition( core::vector3df( xf, yf, zf ) ); 
	if(m_camera->getPosition().Y < 10)
	{
		m_camera->setPosition(vector3df(xf, 10, zf));
	}

	m_camera->setTarget( core::vector3df( playerPos.X, playerPos.Y+25.0f, playerPos.Z ) ); 

	if(!ThePlayer::Instance()->GetDead())
	{
		ThePlayer::Instance()->GetMesh()->GetSceneNode()->setRotation( core::vector3df( 0, m_cameraDirection + 90, 0 ) ); 
	}
}

