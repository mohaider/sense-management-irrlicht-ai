#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include "GameState.h"

class PlayState : public GameState
{
public:

	bool Init();
	void Clean();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update(float dt);
	void Draw();

	static PlayState* Instance() {
		return &m_PlayState;
	}

protected:
	PlayState() { }

private:
	static PlayState m_PlayState;
};

#endif