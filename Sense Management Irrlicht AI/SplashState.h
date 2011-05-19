#ifndef SPLASH_STATE_H
#define SPLASH_STATE_H

#include "GameState.h"

class SplashState : public GameState
{
public:

	bool Init();
	void Clean();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update(float dt);
	void Draw();

	static SplashState* Instance() {
		return &m_SplashState;
	}

protected:
	SplashState() { }

private:
	static SplashState m_SplashState;
};

#endif