#include "SoundModality.h"
#include "Sensor.h"
#include "LuaManager.h"

SoundModality::SoundModality()
{
	Load();
}

void SoundModality::Load()
{
	// Get the values from a Lua File
	LuaPlus::LuaObject luaObj = LuaMan::Instance()->GetLuaStateOwner()->GetGlobal("sound");
	if(luaObj.IsTable())
	{
		m_maxRange = (float)luaObj["MaxRange"].ToNumber();
		m_attenuation = (float)luaObj["Attenuation"].ToNumber();
		m_inverseTransmissionSpeed = (float)luaObj["InverseTransmissionSpeed"].ToNumber();
	}
}

bool SoundModality::ExtraChecks(Signal* signal, Sensor* sensor)
{
	return true;
}