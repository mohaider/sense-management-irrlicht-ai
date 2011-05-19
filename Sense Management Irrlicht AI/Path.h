#ifndef PATH_H
#define PATH_H

#include <list>
#include <math.h>

#include "IrrlichtObject.h"

class Path
{
public:

	Path() : m_bLooped(false) {}

	Path(int NumWayPoints, float MinX, float MinY, float MaxX, float MaxY, bool looped) : m_bLooped(looped)
	{
		CreateRandomPath(NumWayPoints, MinX, MinY, MaxX, MaxY);

		currentWayPoint = m_wayPoints.begin();
	}

	vector2df CurrentWayPoint() const { return *currentWayPoint; }

	bool Finished() { return !(currentWayPoint != m_wayPoints.end()); }

	inline void SetNextWayPoint();

	std::list<vector2df> CreateRandomPath(int NumWayPoints, float MinX, float MinY, float MaxX, float MaxY) {}

	void LoopOn() {m_bLooped = true;}
	void LoopOff() {m_bLooped = false;}

	void AddWayPoint(vector2df newPoint) { m_wayPoints.push_back(newPoint); }

	void Set(std::list<vector2df> newPath) {m_wayPoints = newPath; currentWayPoint = m_wayPoints.begin();}
	void Set(const Path& path) { m_wayPoints = path.GetPath(); currentWayPoint = m_wayPoints.begin();}

	void Clear() {m_wayPoints.clear();}

	std::list<vector2df> GetPath() const { return m_wayPoints; }

	void Render() const;

private:

	std::list<vector2df> m_wayPoints;
	std::list<vector2df>::iterator currentWayPoint;

	bool m_bLooped;
};

inline void Path::SetNextWayPoint()
{
	if(++currentWayPoint == m_wayPoints.end())
	{
		if(m_bLooped)
		{
			currentWayPoint = m_wayPoints.begin();
		}
	}
}

#endif