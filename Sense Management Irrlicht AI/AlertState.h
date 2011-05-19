#ifndef ALERT_STATE_H
#define ALERT_STATE_H

#include "State.h"
#include "AIObject.h"

class AlertState : public State<AIObject>
{
public:

	static const char* s_type;
	static State<AIObject>* CreateAlertState();

	virtual void OnEnter(AIObject* guard);
	virtual void OnExit(AIObject* guard);
	virtual void Execute(AIObject* guard);

	virtual bool OnMessage(AIObject* guard, const Message& msg);

private:

	float g_time;
	double notSpotted;

	AlertState();
};

#endif