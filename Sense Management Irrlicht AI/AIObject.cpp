#include "AIObject.h"
#include "GameObjectFactory.h"
#include "Game.h"
#include "Player.h"
#include "PatrolState.h"
#include "AlertState.h"
#include "SleepState.h"
#include "InvestigateState.h"
#include "HearingSensor.h"
#include "VisionSensor.h"
#include "RegionalSenseManager.h"
#include "LuaManager.h"
#include "StateFactory.h"

const float pi = 3.14159265f;

const char* AIObject::s_type = "AIObject";

static bool a = TheGameObjectFactory::Instance()->AddType("AIObject", AIObject::CreateAIObject);

GameObject* AIObject::CreateAIObject()
{
	return new AIObject;
}

AIObject::AIObject() :	m_pStateMachine(0),
						m_pSteeringBehaviour(0), 
						m_maxViewingDistance(0),
						m_pHearingSensor(0), 
						m_maxForce(0), 
						m_aiListID(0), 
						m_bInAttackRange(false), 
						m_bSleeping(false),
						SteeringForce(vector2df(0,0))
{
	// create this AI's state machine
	m_pStateMachine = new StateMachine<AIObject>(this);
	m_pSteeringBehaviour = new SteeringBehaviour(this);

	// create this AI's hearing sensor
	m_pHearingSensor = new HearingSensor();
	m_pHearingSensor->m_pOwner = this;
	m_pHearingSensor->m_orientation = 0;
	m_pHearingSensor->m_threshold = 100.0f;
	TheRSM::Instance()->AddSensor(m_pHearingSensor);

	// create this AI's vision sensor
	m_pVisionSensor = new VisionSensor();
	m_pVisionSensor->m_pOwner = this;
	m_pVisionSensor->m_orientation = 0;
	m_pVisionSensor->m_threshold = 100.0f;
	TheRSM::Instance()->AddSensor(m_pVisionSensor);

	m_maxForce = 20.0f;
	m_maxSpeed = 80.0f;
}

void AIObject::Reset()
{
	m_pStateMachine->ChangeState(m_stateMap[m_startState]);
	m_position = m_startingPosition;
	m_velocity = vector3df(0,0,0);
	m_pMesh->GetSceneNode()->setAnimationSpeed(20);
	m_pMesh->GetSceneNode()->setPosition(m_position);
	m_pMesh->GetSceneNode()->setLoopMode(true); 
}

bool AIObject::Load(File* pFile)
{
	std::string meshFile;
	std::string texFile;
	float x;
	float y;
	float z;
	int numStates;
	std::string startState;
	std::string globalState;

	if(!pFile->GetString(&meshFile))
	{
		File::ReportError("Could not find player mesh in file");
		return false;
	}

	if(!pFile->GetString(&texFile))
	{
		File::ReportError("Could not find player texture in file");
		return false;
	}

	if(!pFile->GetFloat(&x))
	{
		File::ReportError("Could not find player x position in file");
		return false;
	}

	if(!pFile->GetFloat(&y))
	{
		File::ReportError("Could not find player y position in file");
		return false;
	}

	if(!pFile->GetFloat(&z))
	{
		File::ReportError("Could not find player z position in file");
		return false;
	}

	if(!pFile->GetInt(&numStates))
	{
		return false;
	}

	for(int i = 0; i < numStates; i++)
	{
		std::string typeID;
		if(!pFile->GetString(&typeID))
		{
			return false;
		}

		State<AIObject>* newState = TheStateFactory::Instance()->Create(typeID);
		m_stateMap[typeID] = newState;
	}

	if(!pFile->GetString(&startState))
	{
		return false;
	}

	if(!pFile->GetString(&globalState))
	{
		return false;
	}

	// set states 
	m_pStateMachine->SetCurrentState(m_stateMap[startState]);
	m_pStateMachine->SetGlobalState(m_stateMap[globalState]);

	// create the mesh
	m_pMesh = new IrrlichtMesh();
	m_pMesh->Load(meshFile.c_str(), texFile.c_str(), x, y, z);

	// set position
	m_position = vector3df(x,y,z);
	m_startingPosition = m_position;

	m_startState = startState;

	m_pMesh->GetSceneNode()->setPosition(m_position);

	// set the start state
	m_pStateMachine->GetCurrentState()->OnEnter(this);

	return true;
}

void AIObject::Update(float dt)
{
	// clear the steering force 
	SteeringForce = vector2df(0,0);

	// update the state machine
	m_pStateMachine->Update();

	// calculate the weighted steering force
	SteeringForce = m_pSteeringBehaviour->Calculate();

	// set the AI's velocity based on the steering force
	m_velocity += vector3df(SteeringForce.X, 0, SteeringForce.Y);

	// save the current position
	vector3df oldPos = m_position;

	// do movement
	MovingObject::Update(dt);

	// radians to degrees and atan2 to rotate facing direction
	float angle = atan2(-m_velocity.Z, m_velocity.X);
	float deg = angle * (180/pi);

	// set the AI sensors orientation
	m_pHearingSensor->m_orientation = deg;
	m_pVisionSensor->m_orientation = deg;

	// set the AI's rotation
	m_rotation = deg;

	// if in attack range do the attack animation and kill the player
	if(m_bInAttackRange)
	{
		m_pMesh->GetSceneNode()->setAnimationSpeed(7);
		m_pMesh->GetSceneNode()->setLoopMode(false); // don't loop the animation
		SetAnimation(59, 67); // attack animation

		m_position = oldPos; // stop the AI moving
	}
	else // do the walking animation
	{
		if(m_velocity.X > 0.0 || m_velocity.Z > 0.0 || m_velocity.X < 0.0 || m_velocity.Z < 0.0)
		{
			// walking animation
			SetAnimation(0,13);
		}
		else
		{
			// idle animation
			SetAnimation(205, 249);
		}
	}

	// update positions and rotation for irrlicht
	m_pMesh->GetSceneNode()->setPosition(m_position);
	m_pMesh->GetSceneNode()->setRotation(vector3df(0, deg + 90, 0));

	// update sensors positions
	m_pHearingSensor->m_position = m_position;
	m_pVisionSensor->m_position = m_position;
}

void AIObject::Draw()
{
	// handled by irrlicht
}

bool AIObject::HandleMessage(Message* msg)
{
	// the state machine handles messages
	m_pStateMachine->HandleMessage(msg);
	return true;
}

void AIObject::Clean()
{
	delete m_pStateMachine;
	delete m_pSteeringBehaviour;
	delete m_pVisionSensor;
	delete m_pHearingSensor;
	delete m_pMesh;
}