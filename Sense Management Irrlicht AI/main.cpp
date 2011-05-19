/*
main.cpp
Shaun Mitchell
*/

#include "IrrlichtObject.h"
#include "Game.h"
#include "SplashState.h"
#include "Timer.h"
#include "EventHandler.h"

#include <SDL.h>
#include <SDL_mixer.h>

#ifdef _IRR_WINDOWS_
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#undef main

int main()
{
	SDL_Init(SDL_INIT_AUDIO);

	TheIrrlichtObject::Instance()->Init(L"AI Sense Management", 800, 600, 32, false, true, false, TheEventReceiver::Instance());

	TheGame::Instance()->Init();

	TheGame::Instance()->ChangeState(SplashState::Instance());

	Timer timer;

	while (TheIrrlichtObject::Instance()->Running())
	{
		float dt = timer.timeSinceLastFrame();

		const float MAX_DT = 0.1f;
		if (dt > MAX_DT)
		{
			dt = MAX_DT;
		}

		TheGame::Instance()->HandleEvents();
		TheGame::Instance()->Draw();
		TheGame::Instance()->Update(dt);
	}

	return 0;

}