#include "AlertState.h"
#include "PrecisionTimer.h"
#include "Game.h"
#include "Player.h"
#include "Message.h"
#include "MessageTypes.h"
#include "MessageSender.h"
#include "SoundManager.h"
#include "StateFactory.h"
#include <list>

/*
	Alert State

	The agent will chase the player unless remains outside signal range for a certain time.
	Can receive any message.
*/

const char* AlertState::s_type = "Alert";

static bool a = TheStateFactory::Instance()->AddType("Alert", AlertState::CreateAlertState);

State<AIObject>* AlertState::CreateAlertState()
{
	return new AlertState;
}

AlertState::AlertState()
{
	g_time = 0;
	notSpotted = 0;
}

void AlertState::OnEnter(AIObject* guard)
{
	guard->GetSteering()->FollowPathOff();
	guard->GetSteering()->ArriveOn();
}

void AlertState::OnExit(AIObject* guard)
{
}

void AlertState::Execute(AIObject* guard)
{
	float distance = Vec2Distance(vector2df(ThePlayer::Instance()->GetPosition().X, ThePlayer::Instance()->GetPosition().Z), vector2df(guard->GetPosition().X, guard->GetPosition().Z));
	if(distance < 10)
	{
		guard->m_bInAttackRange = true;
		ThePlayer::Instance()->SetDead(true); // Strange but gives the game a goal, to stay alive!
	}
	else
	{
		guard->m_bInAttackRange = false;
	}

	g_time = (float)TheTimer::Instance()->CurrentTime();

	if(g_time > notSpotted)
	{
		TheMessageSender::Instance()->SendMessageNS(10.0, guard->GetAIID(), guard->GetAIID(), Msg_FalseAlarm);
		g_time = (float)TheTimer::Instance()->CurrentTime();
		notSpotted = TheTimer::Instance()->CurrentTime() + 20;
	}
}

bool AlertState::OnMessage(AIObject* guard, const Message& msg)
{
	switch(msg.Msg)
	{
	case Msg_SpottedEnemy:
		g_time = (float)TheTimer::Instance()->CurrentTime();
		notSpotted = TheTimer::Instance()->CurrentTime() + 20;
		break;

	case Msg_FalseAlarm:
		guard->GetStateMachine()->ChangeState(guard->GetStateMap()["Patrol"]);
		break;

	}
	return false;
}