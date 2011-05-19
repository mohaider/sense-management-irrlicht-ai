#ifndef WALL_H
#define WALL_H

#include "IrrlichtObject.h"

class Wall
{
private:
	vector2df m_a, m_b, m_n;

	void CalculateNormal()
	{
		vector2df n = Vec2DNormalize(m_b - m_a);
		m_n.X = -n.Y;
		m_n.Y = n.X;
	}

public:

	Wall() {}

	Wall(vector2df a, vector2df b) : m_a(a), m_b(b)
	{
		CalculateNormal();
	}

	Wall(vector2df a, vector2df b, vector2df n) : m_a(a), m_b(b), m_n(n)
	{

	}

	vector2df From() const { return m_a; }
	void SetFrom(vector2df v) { m_a = v; CalculateNormal(); }

	vector2df To() const { return m_b; }
	void SetTo(vector2df v) { m_b = v; CalculateNormal(); }

	vector2df Normal() const { return m_n; }
	void SetNormal(vector2df n) { m_n = n; }

	vector2df Center() const { return (m_a + m_b) / 2.0; }
};

#endif