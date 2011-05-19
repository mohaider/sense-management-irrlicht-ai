#ifndef STATE_FACTORY_H
#define STATE_FACTORY_H

/*
	State factory class
*/
#include "State.h"
#include "Factory.h"
#include "Singleton.h"

typedef State<AIObject> AIState;
typedef Factory<AIState> StateFactory;
typedef Singleton<StateFactory> TheStateFactory;

#endif