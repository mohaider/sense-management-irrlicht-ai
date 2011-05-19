#ifndef IRRLICHT_OBJECT_H
#define IRRLICHT_OBJECT_H

/*
	Irrlicht Object Header
	Shaun Mitchell
*/

// The irrlicht header
#include <irrlicht.h>

#include "Singleton.h"

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

// use all the irrlicht namespaces :)
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class IrrlichtObject
{

public:

	~IrrlichtObject();

	bool Init(const wchar_t* caption, int width, int height, int bpp, bool fullscreen, bool stencil, bool vsync, IEventReceiver* eventReceiver);
	void Draw();

	bool Running();

	IrrlichtDevice* GetIrrlichtDevice();
	ISceneManager* GetSceneManager();
	IVideoDriver* GetVideoDriver();
	IGUIEnvironment* GetGUIEnvironment();

private:

	IrrlichtObject() {}
	friend class Singleton<IrrlichtObject>;
};

typedef Singleton<IrrlichtObject> TheIrrlichtObject;

#endif