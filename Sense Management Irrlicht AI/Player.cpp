#include "Player.h"
#include "GameObjectFactory.h"
#include "Game.h"
#include "RegionalSenseManager.h"
#include "EventHandler.h"
#include "SoundManager.h"
#include "Collision.h"
#include "LuaManager.h"

const char* Player::s_type = "Player";

static bool a = TheGameObjectFactory::Instance()->AddType("Player", Player::CreatePlayer);

GameObject* Player::CreatePlayer()
{
	return new Player;
}

Player::Player() :	m_bInLight(false), 
					m_bMoving(false), 
					m_timer(0), 
					m_bDead(false)
{

}

void Player::Reset()
{
	m_position = m_startingPosition;
	m_velocity = vector3df(0,0,0);
	m_bMoveBackward = false;
	m_bMoveForward = false;
	m_bMoveLeft = false;
	m_bMoveRight = false;
	m_bMoving = false;
	m_bRunning = false;
	m_pMesh->GetSceneNode()->setLoopMode(true);
	m_pMesh->GetSceneNode()->setAnimationSpeed(20);
}

bool Player::Load(File* pFile)
{
	std::string meshFile;
	std::string texFile;
	float x;
	float y;
	float z;

	// Get the values from a Lua File
	LuaPlus::LuaObject luaObj = LuaMan::Instance()->GetLuaStateOwner()->GetGlobal("player");
	if(luaObj.IsTable())
	{
		meshFile = luaObj["Mesh"].ToString();
		texFile = luaObj["Texture"].ToString();
		x = (float)luaObj["X"].ToNumber();
		y = (float)luaObj["Y"].ToNumber();
		z = (float)luaObj["Z"].ToNumber();
	}

	m_pMesh = new IrrlichtMesh();
	m_pMesh->Load(meshFile.c_str(), texFile.c_str(), x, y, z);

	m_position = vector3df(x,y,z);
	m_startingPosition = m_position;

	m_pMesh->GetSceneNode()->setPosition(m_position);

	return true;
}

void Player::Update(float dt)
{
	DoAISpecific();
	Move(dt);

	m_timer += dt;

	m_pMesh->GetSceneNode()->setPosition(m_position);
	m_bInLight = false; // this is set in DoAISpecific() and is always false unless within light radius
}

void Player::Draw()
{
	// handled by irrlicht
}

void Player::HandleInput()
{
	if(TheEventReceiver::Instance()->IsKeyDown(KEY_KEY_W))
	{
		m_bMoveBackward = false;
		m_bMoveForward = true;
	}
	else if(TheEventReceiver::Instance()->IsKeyUp(KEY_KEY_W))
	{
		m_bMoveForward = false;
	}

	if(TheEventReceiver::Instance()->IsKeyDown(KEY_KEY_S))
	{
		m_bMoveForward = false;
		m_bMoveBackward = true;
	}
	else if(TheEventReceiver::Instance()->IsKeyUp(KEY_KEY_S))
	{
		m_bMoveBackward = false;
	}

	if(TheEventReceiver::Instance()->IsKeyDown(KEY_KEY_A))
	{
		m_bMoveRight = false;
		m_bMoveLeft = true;
	}
	else if(TheEventReceiver::Instance()->IsKeyUp(KEY_KEY_A))
	{
		m_bMoveLeft = false;
	}

	if(TheEventReceiver::Instance()->IsKeyDown(KEY_KEY_D))
	{
		m_bMoveLeft = false;
		m_bMoveRight = true;
	}
	else if(TheEventReceiver::Instance()->IsKeyUp(KEY_KEY_D))
	{
		m_bMoveRight = false;
	}

	if(TheEventReceiver::Instance()->IsKeyDown(KEY_SPACE))
	{
		m_bRunning = true;
	}
	else if(TheEventReceiver::Instance()->IsKeyUp(KEY_SPACE))
	{
		m_bRunning = false;
	}

	if(TheEventReceiver::Instance()->IsKeyDown(KEY_TAB))
	{
		SoundManager::Instance()->PlaySound("assets/sounds/comeon.wav", 0, 1, 300);
		TheRSM::Instance()->EmitSoundSignal(300.0f, m_position, origin_player);
	}
}

void Player::DoAISpecific() // Emit signals and check for whether in light or not
{
	/*
		check whether the player is in light or not, this is a basic circle, circle intersection test, for speed. 
		There is possibly some way that a real-time check of the current light emitting from an object could be used instead.
	*/
	for(unsigned int i = 0; i < TheGame::Instance()->GetLights().size(); i++)
	{
		if(collision::CircleCircleCollide(vector2df(m_position.X, m_position.Z), m_pMesh->GetSceneNode()->getBoundingBox().getExtent().X, vector2df(TheGame::Instance()->GetLights()[i]->getAbsolutePosition().X, TheGame::Instance()->GetLights()[i]->getAbsolutePosition().Z), 500))
		{
			m_bInLight = true;
		}
	}

	if(m_timer > 0.01 && m_bInLight && !m_bDead) // emit a visual signal every so often // a stronger signal if moving
	{
		if(m_bMoving)
		{
			TheRSM::Instance()->EmitVisualSignal(300.0f, m_position, origin_player);
			m_timer = 0;
		}
		else if(!m_bMoving)
		{
			TheRSM::Instance()->EmitVisualSignal(300.0f, m_position, origin_player);
			m_timer = 0;
		}
	}
}

bool Player::CheckForCollision()
{
	// Check the world for collisions
	for(Game::BoundingBoxes::iterator it = TheGame::Instance()->GetBoundingBoxes().begin(); it != TheGame::Instance()->GetBoundingBoxes().end(); ++it)
	{
		if(m_pMesh->GetSceneNode()->getTransformedBoundingBox().intersectsWithBox((*it)->getTransformedBoundingBox()))
		{
			return true;
		}
	}
	return false;
}

void Player::Move(float dt)
{
	if(!m_bDead)
	{
		if(m_bMoveForward)
		{
			m_bMoving = true;
			if(m_bRunning)
			{
				m_velocity = m_pMesh->GetSceneNode()->getRotation().rotationToDirection() * 70.0f;
			}
			else
			{
				m_velocity = m_pMesh->GetSceneNode()->getRotation().rotationToDirection() * 50.0f;
			}

			if(m_bMoveLeft || m_bMoveRight)
			{
				SetAnimation(14,29);
			}
			else
			{
				SetAnimation(0,13);
			}
		}
		else if(m_bMoveBackward)
		{
			m_bMoving = true;
			if(m_bRunning)
			{
				m_velocity = -m_pMesh->GetSceneNode()->getRotation().rotationToDirection() * 70.0f;
			}
			else
			{
				m_velocity = -m_pMesh->GetSceneNode()->getRotation().rotationToDirection() * 50.0f;
			}

			if(m_bMoveLeft || m_bMoveRight)
			{
				SetAnimation(14,29);
			}
			else
			{
				SetAnimation(0,13);
			}
		}

		if(m_bMoveLeft)
		{
			m_bMoving = true;
			vector3df vel = m_pMesh->GetSceneNode()->getRotation().rotationToDirection();
			m_velocity = vector3df(-vel.Z, 0, vel.X) * 30.0f;
			SetAnimation(14,29);
		}
		else if(m_bMoveRight)
		{
			m_bMoving = true;
			vector3df vel = m_pMesh->GetSceneNode()->getRotation().rotationToDirection();
			m_velocity = vector3df(-vel.Z, 0, vel.X) * -30.0f;
			SetAnimation(14,29);
		}

		if(!m_bMoveBackward && !m_bMoveForward && !m_bMoveLeft && !m_bMoveRight)
		{
			m_bMoving = false;
			SetAnimation(205, 249);
			m_velocity = vector3df(0,0,0);
		}
	}
	else if(m_bDead)
	{
		m_pMesh->GetSceneNode()->setAnimationSpeed(1);
		m_pMesh->GetSceneNode()->setLoopMode(false);
		SetAnimation(167, 172);
		m_velocity = vector3df(0,0,0);
	}

	vector3df oldPos = m_position;

	MovingObject::Update(dt);

	if(CheckForCollision())
	{
		m_position = oldPos;
	}
}

void Player::Clean()
{
	m_pMesh->GetSceneNode()->removeAll();
	delete m_pMesh;
}