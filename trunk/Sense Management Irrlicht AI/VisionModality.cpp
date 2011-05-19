#include "VisionModality.h"
#include "Sensor.h"
#include "Collision.h"
#include "LuaManager.h"

VisionModality::VisionModality() : m_fovAngle(0)
{
	Load();
}

void VisionModality::Load()
{
	// Get the values from a Lua File
	LuaPlus::LuaObject luaObj = LuaMan::Instance()->GetLuaStateOwner()->GetGlobal("vision");
	if(luaObj.IsTable())
	{
		m_maxRange = (float)luaObj["MaxRange"].ToNumber();
		m_attenuation = (float)luaObj["Attenuation"].ToNumber();
		m_inverseTransmissionSpeed = (float)luaObj["InverseTransmissionSpeed"].ToNumber();
		m_fovAngle = (float)luaObj["FieldOfViewAngle"].ToNumber();
	}
}

// Some extra vision specific checks
bool VisionModality::ExtraChecks(Signal* signal, Sensor* sensor)
{
	// Check if vision signal is within FOV
	vector3df vec = signal->m_position - sensor->m_pOwner->GetPosition();
	vec.normalize();

	vector3df facing = sensor->m_pOwner->GetMesh()->GetSceneNode()->getRotation().rotationToDirection();
	float viewScaler = facing.dotProduct(vec);

	if(viewScaler <= cos(m_fovAngle * 0.5f))
	{
		return false;
	}

	// Check if vision signal is within LOS
	for (unsigned int w = 0; w < TheGame::Instance()->GetWalls().size(); w++)
	{
		vector2df A = vector2df(signal->m_position.X, signal->m_position.Z);
		vector2df B = vector2df(sensor->m_position.X, sensor->m_position.Z);

		if(collision::LineIntersection2D(A, B, TheGame::Instance()->GetWalls()[w]->From(), TheGame::Instance()->GetWalls()[w]->To()))
		{
			return false;
		}
	}

	// If both checks pass then return true
	return true;
}