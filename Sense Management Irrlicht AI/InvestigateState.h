#ifndef INVESTIGATE_STATE_H
#define INVESTIGATE_STATE_H

#include "State.h"
#include "AIObject.h"

class InvestigateState : public State<AIObject>
{
public:

	static const char* s_type;
	static State<AIObject>* CreateInvestigateState();

	virtual void OnEnter(AIObject* guard);
	virtual void OnExit(AIObject* guard);
	virtual void Execute(AIObject* guard);

	virtual bool OnMessage(AIObject* guard, const Message& msg);

private:

	InvestigateState();

	double m_searchTimer;
};

#endif