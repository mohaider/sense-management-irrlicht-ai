#ifndef PLAYER_H
#define PLAYER_H

#include "MovingObject.h"
#include "IrrlichtMesh.h"
#include "Singleton.h"

class Player : public MovingObject
{
public:

	virtual ~Player() {}

	static const char* s_type;
	static GameObject* CreatePlayer();

	virtual const char* GetTypeName() const { return "Player"; }

	virtual bool Load(File* pFile);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();
	virtual void Reset();


	void HandleInput();
	bool GetDead() { return m_bDead; }
	void SetDead(bool val) { m_bDead = val; }

private:

	Player();
	friend class Singleton<Player>;

	void DoAISpecific();
	bool CheckForCollision();
	void Move(float dt);

	bool m_bInLight;
	bool m_bMoving;
	float m_timer;
	bool m_bDead;
};

typedef Singleton<Player> ThePlayer;

#endif