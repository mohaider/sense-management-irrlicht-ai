#include "IrrlichtObject.h"

static IrrlichtDevice *s_pIrrlichtDevice = 0;
static video::IVideoDriver* s_pVideoDriver = 0;
static scene::ISceneManager* s_pSceneManager = 0;
static gui::IGUIEnvironment* s_pGUIEnvironment = 0;

IrrlichtObject::~IrrlichtObject()
{
    s_pIrrlichtDevice->drop();
}

ISceneManager* IrrlichtObject::GetSceneManager()
{
   return s_pSceneManager;
}

IVideoDriver* IrrlichtObject::GetVideoDriver()
{
  return s_pVideoDriver;
}

IrrlichtDevice* IrrlichtObject::GetIrrlichtDevice()
{
   return s_pIrrlichtDevice;
}

IGUIEnvironment* IrrlichtObject::GetGUIEnvironment()
{
	return s_pGUIEnvironment;
}


bool IrrlichtObject::Init(const wchar_t* caption, int width, int height, int bpp, bool fullscreen, bool stencil, bool vsync, IEventReceiver* eventReceiver)
{
	#ifdef _IRR_WINDOWS_
	s_pIrrlichtDevice = createDevice(EDT_DIRECT3D9, dimension2d<u32>(width, height), bpp, fullscreen, stencil, vsync, eventReceiver);
	#else
	s_pIrrlichtDevice = createDevice(EDT_OPENGL, dimension2d<s32>(width, height), bpp, fullscreen, stencil, vsync, eventReceiver);
	#endif

	s_pVideoDriver = s_pIrrlichtDevice->getVideoDriver();
	s_pSceneManager = s_pIrrlichtDevice->getSceneManager();
	s_pGUIEnvironment = s_pIrrlichtDevice->getGUIEnvironment();

	s_pIrrlichtDevice->setWindowCaption(caption);

	s_pVideoDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	s_pSceneManager->addCameraSceneNodeFPS();

	return true;
}

void IrrlichtObject::Draw()
{
	s_pVideoDriver->beginScene(true, true, video::SColor(255,0,0,0));
    s_pSceneManager->drawAll();
    s_pVideoDriver->endScene();
}

bool IrrlichtObject::Running()
{
	return (s_pIrrlichtDevice->run() && s_pVideoDriver);
}