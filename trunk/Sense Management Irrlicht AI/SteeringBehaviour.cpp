#include "SteeringBehaviour.h"
#include "AIObject.h"
#include "Collision.h"
#include "Player.h"
#include "Game.h"
#include <math.h>
#include <stdlib.h>

SteeringBehaviour::SteeringBehaviour(AIObject* owner) : m_pOwner(owner), m_iFlags(0), m_weightSeek(0.1f), m_weightFlee(0.1f), m_wayPointSeekDistSq(100.0), m_target(vector2df(0,0)), m_steeringForce(vector2df())
{
	m_pPath = new Path();
	m_pPath->LoopOn();
}

SteeringBehaviour::~SteeringBehaviour() {}


vector2df SteeringBehaviour::Calculate()
{
	m_steeringForce.X = 0;
	m_steeringForce.Y = 0;

	m_steeringForce = CalculatePrioritized();

	return m_steeringForce;
}

bool SteeringBehaviour::AccumulateForce(vector2df &sf, vector2df ForceToAdd)
{
	float lengthSoFar = sf.getLength();

	float lengthRemaining = m_pOwner->GetMaxForce() - lengthSoFar;

	if(lengthRemaining <= 0.0) return false;

	float lengthToAdd = ForceToAdd.getLength();

	if(lengthToAdd < lengthRemaining)
	{
		sf += ForceToAdd;
	}
	else
	{
		sf += ForceToAdd.normalize() * lengthRemaining;
	}

	return true;
}

vector2df SteeringBehaviour::CalculatePrioritized()
{
	vector2df force;

	if(On(Steering_wallavoidance))
	{
		force = WallAvoidance(TheGame::Instance()->GetWalls());

		if(!AccumulateForce(m_steeringForce, force)) return m_steeringForce;
	}

	if(On(Steering_separation))
	{
		force = Separation(TheGame::Instance()->GetAIObjects());

		if(!AccumulateForce(m_steeringForce, force)) return m_steeringForce;
	}

	if(On(Steering_follow_path))
	{
		force = FollowPath();

		if(!AccumulateForce(m_steeringForce, force)) return m_steeringForce;
	}

	if(On(Steering_arrive))
	{
		force = Arrive(m_target, fast);

		if(!AccumulateForce(m_steeringForce, force)) return m_steeringForce;
	}

	if(On(Steering_seek))
	{
		force = Seek(m_target) * m_weightSeek;

		if(!AccumulateForce(m_steeringForce, force)) return m_steeringForce;
	}

	return m_steeringForce;
}

vector2df SteeringBehaviour::Seek(vector2df target)
{
	vector2df newvel = vector2df(target - vector2df(m_pOwner->GetPosition().X, m_pOwner->GetPosition().Z));

	vector2df DesiredVelocity = newvel.normalize() * m_pOwner->GetMaxSpeed();

	vector2df desvel = DesiredVelocity - vector2df(m_pOwner->GetVelocity().X, m_pOwner->GetVelocity().Z);

	return desvel;
}

vector2df SteeringBehaviour::Arrive(vector2df target, Deceleration deceleration)
{
	vector2df toTarget = target - vector2df(m_pOwner->GetPosition().X, m_pOwner->GetPosition().Z);

	float distance = toTarget.getLength();

	if(distance > 0.000001f)
	{
		const float decel = 0.2f;

		float speed = distance / (1 * decel);

		speed = std::min(speed, m_pOwner->GetMaxSpeed());

		vector2df newVel = toTarget * speed / distance;

		return (newVel - vector2df(m_pOwner->GetVelocity().X, m_pOwner->GetVelocity().Z));
	}

	return vector2df(0,0);
}

vector2df SteeringBehaviour::FollowPath()
{
	if(m_pPath->CurrentWayPoint().getDistanceFromSQ(vector2df(m_pOwner->GetPosition().X, m_pOwner->GetPosition().Z)) < m_wayPointSeekDistSq)
	{
		m_pPath->SetNextWayPoint();
	}

	if(!m_pPath->Finished())
	{
		return Seek(m_pPath->CurrentWayPoint());
	}
	else
	{
		return Arrive(m_pPath->CurrentWayPoint(), normal);
	}
}

vector2df SteeringBehaviour::WallAvoidance(const std::vector<Wall*>& walls)
{
	m_feelers[0] = vector2df(m_pOwner->GetVelocity().X,m_pOwner->GetVelocity().Z) * 1.5f;
	m_feelers[0] += vector2df(m_pOwner->GetPosition().X, m_pOwner->GetPosition().Z);

	m_feelers[1] = vector2df(m_pOwner->GetVelocity().X,m_pOwner->GetVelocity().Z) * 0.5f;
	m_feelers[1].rotateBy(80, vector2df(m_pOwner->GetPosition().X, m_pOwner->GetPosition().Z));
	m_feelers[1] += vector2df(m_pOwner->GetPosition().X, m_pOwner->GetPosition().Z);

	m_feelers[2] = vector2df(m_pOwner->GetVelocity().X,m_pOwner->GetVelocity().Z) * 0.5f;
	m_feelers[2].rotateBy(45, vector2df(m_pOwner->GetPosition().X, m_pOwner->GetPosition().Z));
	m_feelers[2] += vector2df(m_pOwner->GetPosition().X, m_pOwner->GetPosition().Z);

	float DistToThisIP = 0.0f;
	float DistToClosestIP = 9999999999.f;

	//this will hold an index into the vector of walls
	int ClosestWall = -1;

	vector2df Point;         //used for storing temporary info
	vector2df ClosestPoint;  //holds the closest intersection point
	vector2df SteeringForce;

	for (unsigned int w = 0; w < walls.size(); w++)
	{
		if (collision::LineIntersection2D(vector2df(m_pOwner->GetPosition().X, m_pOwner->GetPosition().Z),
			m_feelers[0],
			walls[w]->From(),
			walls[w]->To(),
			DistToThisIP,
			Point))
		{
			//want to keep a record of the closest wall to the unit
			//check to see if the dist to the wall is less than smallest intersection dist
			if (DistToThisIP < DistToClosestIP)
			{
				DistToClosestIP = DistToThisIP;

				ClosestWall = w;

				ClosestPoint = Point;
			}
		}
	}//next wall


	//if an intersection point has been detected, calculate a force  
	//that will direct the agent away
	if (ClosestWall >= 0)
	{
		//calculate by what distance the projected position of the agent
		//will overshoot the wall
		vector2df OverShoot = m_feelers[0] - ClosestPoint;
		SteeringForce = walls[ClosestWall]->Normal() * OverShoot.getLength();

		//cal normal
		vector2df temp1 = (walls[ClosestWall]->To() - walls[ClosestWall]->From());
		vector2df temp(temp1.X,temp1.Y);
		temp.normalize();
		vector2df temp3 = temp;
		vector2df normal(temp.Y,temp.X);
		temp3 = normal;

		//create a force in the direction of the wall normal, with a 
		//magnitude of the overshoot
		//- normal??
		SteeringForce = -normal * 1.0f * m_pOwner->GetMaxSpeed();
		SteeringForce.Y = -SteeringForce.Y;
	}

	//next Prob

	return SteeringForce;
}

vector2df SteeringBehaviour::Separation(std::map<int, AIObject*> neighbors)
{  
	vector2df SteeringForce;

	for (Game::AIObjects::iterator it = neighbors.begin(); it != neighbors.end(); it++)
	{
		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough.
		AIObject* pAIObj = it->second;

		float distance = Vec2Distance(vector2df(m_pOwner->GetPosition().X,m_pOwner->GetPosition().Z), vector2df(pAIObj->GetPosition().X, pAIObj->GetPosition().Z));

		if((pAIObj != m_pOwner) && distance < 30)
		{
			vector2df ToAgent = vector2df(m_pOwner->GetPosition().X,m_pOwner->GetPosition().Z) - vector2df(pAIObj->GetPosition().X, pAIObj->GetPosition().Z);

			//scale the force inversely proportional to the agent's distance
			//from its neighbor.
			SteeringForce += Vec2DNormalize(ToAgent)/ToAgent.getLength();
		}

		return SteeringForce;
	}
}




