#ifndef MOVING_OBJECT_H
#define MOVING_OBJECT_H

#include "GameObject.h"
#include "IrrlichtObject.h"
#include "IrrlichtMesh.h"

class MovingObject : public GameObject
{

public:

	MovingObject();
	virtual ~MovingObject() {float f = 0;}

	virtual const char* GetTypeName() const { return "MovingObject"; }

	virtual bool Load(File* pFile);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();
	virtual void Reset() {}

	// accessors
	vector3df GetPosition() const { return m_position; }
	vector3df GetVelocity() const { return m_velocity; }
	vector3df GetAcceleration() const { return m_acceleration; }

	void SetPosition(const vector3df& position) { m_position = position; }
	void SetPosition(float x, float y, float z) { m_position = vector3df(x,y,z); }

	void SetVelocity(const vector3df& velocity) { m_velocity = velocity; }
	void SetVelocity(float x, float y, float z) { m_velocity = vector3df(x,y,z); }

	void SetAcceleration(const vector3df& acceleration) { m_acceleration = acceleration; }
	void SetAcceleration(float x, float y, float z) { m_acceleration = vector3df(x,y,z); }

	void SetAnimation(int start, int end);
	IrrlichtMesh* GetMesh() const { return m_pMesh; }

protected:

	IrrlichtMesh* m_pMesh;

	vector3df m_position;
	vector3df m_velocity;
	vector3df m_acceleration;

	vector3df m_startingPosition; // store the starting position

	bool m_bMoveForward;
	bool m_bMoveBackward;
	bool m_bMoveLeft;
	bool m_bMoveRight;
	bool m_bRunning;
};

#endif