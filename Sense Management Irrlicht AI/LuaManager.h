#ifndef _LUA_MANAGER_H_
#define _LUA_MANAGER_H_

#include "Singleton.h"
#pragma comment (lib, "luaplus51-1200.lib")
#include <LuaPlus.h>

class LuaManager
{
public:

	~LuaManager() {}

	LuaPlus::LuaState* GetLuaStateOwner() { return m_stateOwner.Get(); }

	void OpenFiles();

private:

	LuaManager() { OpenFiles(); }
	friend class Singleton<LuaManager>;

	LuaPlus::LuaStateOwner m_stateOwner;
	LuaPlus::LuaObject m_luaObject;
};

typedef Singleton<LuaManager> LuaMan;

#endif