#include "LuaManager.h"

void LuaManager::OpenFiles()
{
	m_stateOwner->DoFile("lua files/AI_config.txt");
	m_stateOwner->DoFile("assets/player.txt");
}