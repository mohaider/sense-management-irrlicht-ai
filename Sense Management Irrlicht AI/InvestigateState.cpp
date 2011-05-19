#include "InvestigateState.h"
#include "Message.h"
#include "MessageTypes.h"
#include "MessageSender.h"
#include "StateFactory.h"
#include "Game.h"

const char* InvestigateState::s_type = "Investigate";

static bool a = TheStateFactory::Instance()->AddType("Investigate", InvestigateState::CreateInvestigateState);

State<AIObject>* InvestigateState::CreateInvestigateState()
{
	return new InvestigateState;
}

InvestigateState::InvestigateState() : m_searchTimer(0)
{
}

void InvestigateState::OnEnter(AIObject* guard)
{
	guard->GetSteering()->FollowPathOff();
	guard->GetSteering()->ArriveOn();
}

void InvestigateState::OnExit(AIObject* guard)
{
}

void InvestigateState::Execute(AIObject* guard)
{
	if(m_searchTimer > 1000)
	{
		TheMessageSender::Instance()->SendMessageNS(0, guard->GetAIID(), guard->GetAIID(), Msg_FalseAlarm);
		m_searchTimer = 0;
	}
	m_searchTimer++;

}

bool InvestigateState::OnMessage(AIObject* guard, const Message& msg)
{
	
	switch(msg.Msg)
	{
	case Msg_SpottedEnemy:
		return false;
		break;

	case Msg_FalseAlarm:
		guard->GetStateMachine()->ChangeState(guard->GetStateMap()["Patrol"]);
		break;

	}

	return false;
}