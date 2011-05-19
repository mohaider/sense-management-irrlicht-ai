#ifndef PATROL_STATE_H
#define PATROL_STATE_H

#include "State.h"
#include "AIObject.h"

class PatrolState : public State<AIObject>
{
public:

	static const char* s_type;
	static State<AIObject>* CreatePatrolState();

	virtual void OnEnter(AIObject* guard);
	virtual void OnExit(AIObject* guard);
	virtual void Execute(AIObject* guard);

	virtual bool OnMessage(AIObject* guard, const Message& msg);

private:

	PatrolState();
};

#endif