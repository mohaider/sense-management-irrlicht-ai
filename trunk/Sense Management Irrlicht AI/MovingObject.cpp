#include "MovingObject.h"

MovingObject::MovingObject() : 
								m_position(vector3df()), 
								m_velocity(vector3df()), 
								m_acceleration(vector3df()), 
								m_pMesh(0), 
								m_bMoveBackward(false), 
								m_bMoveForward(false),
								m_bMoveLeft(false),
								m_bMoveRight(false),
								m_bRunning(false)
{

}

bool MovingObject::Load(File* pFile)
{
	return true;
}

void MovingObject::Update(float dt)
{
	m_position += m_velocity * dt;
}

void MovingObject::Draw()
{
	// empty
}

void MovingObject::Clean()
{
	// empty
}

void MovingObject::SetAnimation(int start, int end)
{
	// dont set animation if already set
	if(start == m_pMesh->GetSceneNode()->getStartFrame() && end == m_pMesh->GetSceneNode()->getEndFrame())
	{
		return;
	}

	m_pMesh->GetSceneNode()->setFrameLoop(start, end);
}