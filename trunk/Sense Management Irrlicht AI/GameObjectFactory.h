#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H

#include "Factory.h"
#include "Singleton.h"
#include "GameObject.h"

typedef Factory<GameObject> GameObjectFactory;
typedef Singleton<GameObjectFactory> TheGameObjectFactory;

#endif