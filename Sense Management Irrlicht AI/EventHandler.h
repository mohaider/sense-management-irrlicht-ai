#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "IrrlichtObject.h"
#include "Singleton.h"

class MyEventReceiver : public IEventReceiver
{
public:

	struct SMouseState
	{
		core::position2di Position;
		bool LeftButtonDown;
		SMouseState() : LeftButtonDown(false) { }
	} MouseState;

	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsUp[event.KeyInput.Key] = !event.KeyInput.PressedDown;

		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			switch(event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN:
				MouseState.LeftButtonDown = true;
				break;

			case EMIE_LMOUSE_LEFT_UP:
				MouseState.LeftButtonDown = false;
				break;

			case EMIE_MOUSE_MOVED:
				MouseState.Position.X = event.MouseInput.X;
				MouseState.Position.Y = event.MouseInput.Y;
				break;

			default:
				// We won't use the wheel
				break;
			}
		}
			

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) //const
	{
		return KeyIsDown[keyCode];
	}

	virtual bool IsKeyUp(EKEY_CODE keyCode) //const
	{
		return KeyIsUp[keyCode];
	}

	virtual bool IsLeftMouseDown() const
	{
		return MouseState.LeftButtonDown;
	}

	virtual bool IsLeftMouseUp() const
	{
		return MouseState.LeftButtonDown == false;
	}

private:

	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

	friend class Singleton<MyEventReceiver>;

	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool KeyIsUp[KEY_KEY_CODES_COUNT];
};

typedef Singleton<MyEventReceiver> TheEventReceiver;

#endif