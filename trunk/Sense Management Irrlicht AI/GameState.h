#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

/*
	GameState.h
	Shaun Mitchell
*/

#include "Game.h"

class GameState
{
public:

	virtual bool Init() = 0;
	virtual void Clean() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;

	void ChangeState(GameState* state)
	{
		TheGame::Instance()->ChangeState(state);
	}

protected:
	GameState() {}
};

#endif