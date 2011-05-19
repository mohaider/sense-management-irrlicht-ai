#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

/*
	Interface class for Game Objects
*/

#include "File.h"

class GameObject
{
public:
	virtual const char* GetTypeName() const = 0;

	GameObject() : m_id(0) {}
	virtual ~GameObject() {}

	virtual bool Load(File* pFile) = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual void Clean() = 0;
	virtual void Reset() = 0;

	int GetID() { return m_id; }

protected:

	int m_id;
};

#endif