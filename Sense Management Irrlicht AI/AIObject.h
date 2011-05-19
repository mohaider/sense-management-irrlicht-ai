#ifndef AI_OBJECT_H
#define AI_OBJECT_H

//#include "GameObject.h"
#include "MovingObject.h"
#include <map>
#include <string>
#include <list>

#include "StateMachine.h"
#include "SteeringBehaviour.h"

class Sensor;

class AIObject : public MovingObject
{
public:

	AIObject();
	~AIObject() {}

	typedef std::map<std::string, State<AIObject>*> StateMap;

	std::list<vector2df> m_patrolRoute;
	std::list<vector2df>& GetPatrolRoute() { return m_patrolRoute; }

	static const char* s_type;
	static GameObject* CreateAIObject();

	virtual const char* GetTypeName() const { return "AIObject"; }

	virtual bool Load(File* pFile);

	void SetID(int id) { m_aiListID = id; }
	int GetAIID() { return m_aiListID; }

	virtual bool HandleMessage(Message* msg);

	virtual void Update(float dt);
	virtual void Draw();
	virtual void Clean();
	virtual void Reset();

	SteeringBehaviour* GetSteering() const { return m_pSteeringBehaviour; }

	StateMachine<AIObject>* GetStateMachine() { return m_pStateMachine; }
	StateMap GetStateMap() { return m_stateMap; }

	float GetRotation() { return m_rotation; }
	float GetMaxForce() const { return m_maxForce; }
	float GetMaxSpeed() { return m_maxSpeed; }

	float Sleeping() { return m_bSleeping; }


	bool m_bInAttackRange;
	bool m_bSleeping;

private:

	StateMachine<AIObject>* m_pStateMachine;

	StateMap m_stateMap;

	SteeringBehaviour* m_pSteeringBehaviour;

	float m_maxViewingDistance;

	int startSceneNode;

	float m_rotation;
	float m_maxForce;
	float m_maxSpeed;
	// sensors

	Sensor* m_pHearingSensor;
	Sensor* m_pVisionSensor;

	vector2df SteeringForce;

	std::string m_startState;

	int m_aiListID; // id in the ai list for messages

};

#endif