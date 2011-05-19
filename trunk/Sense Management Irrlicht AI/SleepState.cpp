#include "SleepState.h"
#include "Game.h"
#include "Message.h"
#include "MessageTypes.h"
#include "SoundManager.h"
#include "StateFactory.h"
#include <list>

/*
	Sleep State

	The agent doesn't respond to any messages apart from sounds. The agent will not spot the player if he moves
	within signal range.
*/

const char* SleepState::s_type = "Sleep";

static bool a = TheStateFactory::Instance()->AddType("Sleep", SleepState::CreateSleepState);

State<AIObject>* SleepState::CreateSleepState()
{
	return new SleepState;
}

SleepState::SleepState()
{
}

void SleepState::OnEnter(AIObject* guard)
{
	guard->SetAnimation(183,204);
	guard->m_bSleeping = true;
	guard->GetSteering()->FollowPathOff();
	guard->GetSteering()->SeekOff();
	guard->GetSteering()->ArriveOff();
}

void SleepState::OnExit(AIObject* guard)
{
	guard->m_bSleeping = false;
}

void SleepState::Execute(AIObject* guard)
{
}

bool SleepState::OnMessage(AIObject* guard, const Message& msg)
{
	switch(msg.Msg)
	{
	case Msg_SpottedEnemy:
		break;

	case Msg_SawSomething:
		break;

	case Msg_HeardSomething:
		guard->GetSteering()->m_target = vector2df(msg.signal.m_position.X, msg.signal.m_position.Z);
		guard->GetStateMachine()->ChangeState(guard->GetStateMap()["Alert"]);
		break;

	}
	return false;
}