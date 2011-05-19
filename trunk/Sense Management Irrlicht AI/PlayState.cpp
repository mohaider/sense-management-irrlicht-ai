
#include <stdio.h>

#include "Game.h"
#include "PlayState.h"
#include "GameObjectFactory.h"
#include "Player.h"
#include "AIObject.h"
#include "EventHandler.h"
#include "MessageSender.h"
#include "SoundManager.h"
#include "SplashState.h"
#include "Collision.h"
#include "RegionalSenseManager.h"
#include <assert.h>
#include <iostream>

PlayState PlayState::m_PlayState;

bool PlayState::Init()
{
	// Drawn when the player is dead
	TheIrrlichtObject::Instance()->GetIrrlichtDevice()->getGUIEnvironment()->addImage(
		TheIrrlichtObject::Instance()->GetVideoDriver()->getTexture("assets/textures/blood.png"),
		core::position2d<s32>(0,0));

	// Create the walls for wall avoidance
	for(unsigned int i = 0; i < TheGame::Instance()->GetBoundingBoxes().size(); ++i )
	{
		collision::Create4WallsFrombox(TheGame::Instance()->GetBoundingBoxes()[i]->getTransformedBoundingBox());
	}

	// Start the music and ambient sound effects
	SoundManager::Instance()->PlayMusic("assets/sounds/music1.wav");
	SoundManager::Instance()->PlaySound("assets/sounds/crickets.wav", -1, -1, 100);

	// hide the mouse cursor
	TheIrrlichtObject::Instance()->GetIrrlichtDevice()->getCursorControl()->setVisible(false);

	// load the player object
	ThePlayer::Instance()->Load(NULL);

	// load all the game objects from a file
	File f;
	if (!f.Open("assets/config.txt"))
	{
		return false;
	}

	while (!f.EndOfFile())
	{
		std::string typeName;
		f.GetString(&typeName);
		if (typeName.empty() || typeName[0] == '#')
		{
			return true;
		}
		
		std::cout<< typeName << " Type Object Created" << "\n";

		GameObject* pMyNewObject = 
			TheGameObjectFactory::Instance()->Create(typeName);
		if (!pMyNewObject)
		{
			return false;
		}
		if (!pMyNewObject->Load(&f))
		{
			return false;
		}
		int id = TheGame::Instance()->GetNextID();
		TheGame::Instance()->AddGameObject(id, pMyNewObject);
		TheGame::Instance()->IncrementNextID();

		// If its an AI object then put it in the AI map 
		if(pMyNewObject->GetTypeName() == "AIObject")
		{
			int ai_id = TheGame::Instance()->GetNextID_AI();
			AIObject* aiObject = dynamic_cast<AIObject*>(pMyNewObject);
			aiObject->SetID(ai_id);
			TheGame::Instance()->AddAIObject(ai_id, aiObject);
			TheGame::Instance()->IncrementNextID_AI();
		}
	}

	printf("Main Game State Initialised Succesfully");
	return true;
}

// Free textures and memory
void PlayState::Clean()
{
}

// Pause this state
void PlayState::Pause()
{
}

// Resume this state
void PlayState::Resume()
{
}

// Handle events // player input and exit
void PlayState::HandleEvents()
{
	ThePlayer::Instance()->HandleInput();

	if(TheEventReceiver::Instance()->IsKeyDown(KEY_ESCAPE))
	{
		exit(0);
	}

	if(TheEventReceiver::Instance()->IsKeyDown(KEY_RETURN))
	{
		// when the player dies, press enter to reset and try again
		if(ThePlayer::Instance()->GetDead())
		{
			for(Game::GameObjects::iterator it = TheGame::Instance()->GetGameObjects().begin(); it != TheGame::Instance()->GetGameObjects().end(); ++it)
			{
				GameObject* p = it->second;
				p->Reset();
			}

			ThePlayer::Instance()->Reset();
			ThePlayer::Instance()->SetDead(false);
		}
	}

}

// Update the state // and player
void PlayState::Update(float dt) 
{
	for(Game::GameObjects::const_iterator it = TheGame::Instance()->GetGameObjects().begin(); it != TheGame::Instance()->GetGameObjects().end(); ++it)
	{
		GameObject* p = it->second;
		p->Update(dt);
	}
	ThePlayer::Instance()->Update(dt);

	// send AI messages that are in queue
	TheMessageSender::Instance()->SendDelayedMessages();

	// update the sound manager
	SoundManager::Instance()->Update();

	// move the camera
	TheGame::Instance()->MoveCameraControl();
}

// Render the state // this is done through the scene graph of irrlicht
void PlayState::Draw() 
{
	TheIrrlichtObject::Instance()->GetVideoDriver()->beginScene(true, true, SColor(255, 0, 0, 0));
	TheIrrlichtObject::Instance()->GetSceneManager()->drawAll();

	// if the player is dead, draw the game over screen
	if(ThePlayer::Instance()->GetDead())
	{
		TheIrrlichtObject::Instance()->GetIrrlichtDevice()->getGUIEnvironment()->drawAll();
	}

	TheIrrlichtObject::Instance()->GetVideoDriver()->endScene();
}
