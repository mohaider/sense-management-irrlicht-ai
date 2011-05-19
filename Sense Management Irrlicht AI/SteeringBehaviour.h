#ifndef STEERING_BEHAVIOUR_H
#define STEERING_BEHAVIOUR_H

#include "Path.h"
#include <vector>
#include <map>

class AIObject;
class GameObject;
class Wall;

enum steering_behaviour
{
	Steering_none = 0x00000,
	Steering_seek = 0x00002,
	Steering_arrive = 0x00004,
	Steering_follow_path = 0x00006, 
	Steering_wallavoidance = 0x00008,
	Steering_separation = 0x00010,
};

class SteeringBehaviour
{
public:

	SteeringBehaviour(AIObject* owner);
	virtual ~SteeringBehaviour();

	vector2df Calculate();

	void SetTarget(const vector2df t) { m_target = t; }

	vector2df GetForce() const { return m_steeringForce; }

	void  SetPath(std::list<vector2df> new_path){m_pPath->Set(new_path);}

	void SeekOn(){m_iFlags |= Steering_seek;}
	void ArriveOn(){m_iFlags |= Steering_arrive;}
	void FollowPathOn(){m_iFlags |= Steering_follow_path;}
	void WallAvoidanceOn(){m_iFlags |= Steering_wallavoidance;}
	void SeparationOn(){m_iFlags |= Steering_separation;}

	bool isSeekOn() { return On(Steering_seek); }
	bool isArriveOn() { return On(Steering_arrive); }
	bool isFollowPathOn() { return On(Steering_follow_path); }
	bool isWallAvoidanceOn() { return On(Steering_wallavoidance); }
	bool isSeparationOn() { return On(Steering_separation); }

	void SeekOff() {if(On(Steering_seek)) m_iFlags ^=Steering_seek; }
	void ArriveOff() {if(On(Steering_arrive)) m_iFlags ^=Steering_arrive; }
	void FollowPathOff() {if(On(Steering_follow_path)) m_iFlags ^=Steering_follow_path; }
	void WallAvoidanceOff() {if(On(Steering_wallavoidance)) m_iFlags ^=Steering_wallavoidance; }
	void SeparationOff() {if(On(Steering_separation)) m_iFlags ^=Steering_separation; }

	vector2df Seek(vector2df target);

	vector2df Separation(std::map<int, AIObject*> neighbors);

	enum Deceleration {slow = 3, normal = 2, fast = 1 };
	vector2df Arrive(vector2df target, Deceleration deceleration);

	Path* GetPath() { return m_pPath;}

	Path* m_pPath;
	vector2df FollowPath();

	float m_boxLength;

	vector2df m_feelers[3];

	vector2df m_target;

	vector2df WallAvoidance(const std::vector<Wall*>& walls);

private:

	AIObject* m_pOwner;

	double m_wayPointSeekDistSq;

	vector2df m_steeringForce;

	GameObject* m_pTarget1;
	GameObject* m_pTarget2;

	float m_weightSeek;
	float m_weightFlee;

	bool On(steering_behaviour st) { return (m_iFlags & st) == st; }
	bool AccumulateForce(vector2df &sf, vector2df ForceToAdd);

	vector2df CalculatePrioritized();

	int m_iFlags;
};

#endif