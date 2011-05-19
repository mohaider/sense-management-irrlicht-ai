#include "PatrolState.h"
#include "Game.h"
#include "Message.h"
#include "MessageTypes.h"
#include "SoundManager.h"
#include "StateFactory.h"
#include <list>

/*
	Patrol State

	Agents follow a set path.

	Can receive any message.
*/

const char* PatrolState::s_type = "Patrol";

static bool a = TheStateFactory::Instance()->AddType("Patrol", PatrolState::CreatePatrolState);

State<AIObject>* PatrolState::CreatePatrolState()
{
	return new PatrolState;
}

PatrolState::PatrolState()
{
}

void PatrolState::OnEnter(AIObject* guard)
{
	guard->m_bInAttackRange = false;
	guard->GetSteering()->ArriveOff();
	guard->GetSteering()->WallAvoidanceOn();
	guard->GetSteering()->SetPath(TheGame::Instance()->GetPatrolPath());
	guard->GetSteering()->FollowPathOn();
}

void PatrolState::OnExit(AIObject* guard)
{
	SoundManager::Instance()->PlaySound("assets/sounds/huh.wav", 0, 1, 200);
	guard->GetSteering()->FollowPathOff();
}

void PatrolState::Execute(AIObject* guard)
{
}

bool PatrolState::OnMessage(AIObject* guard, const Message& msg)
{
	switch(msg.Msg)
	{
	case Msg_SpottedEnemy:
		if(!guard->Sleeping())
		{
			guard->GetSteering()->m_target = vector2df(msg.signal.m_position.X, msg.signal.m_position.Z); // send the position that the enemy was spotted
			guard->GetStateMachine()->ChangeState(guard->GetStateMap()["Alert"]); // change to alert state
		}
		break;

	case Msg_SawSomething:
		if(!guard->Sleeping())
		{
			guard->GetSteering()->m_target = vector2df(msg.signal.m_position.X, msg.signal.m_position.Z); // the position that the enemy was spotted
			guard->GetStateMachine()->ChangeState(guard->GetStateMap()["Investigate"]);
		}
		break;

	case Msg_HeardSomething:
		guard->GetSteering()->m_target = vector2df(msg.signal.m_position.X, msg.signal.m_position.Z);
		guard->GetStateMachine()->ChangeState(guard->GetStateMap()["Alert"]);
		break;

	}
	return false;
}