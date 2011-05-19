#ifndef COLLISION_H
#define COLLISION_H

/*
A collection of useful functions
*/

#include "Game.h"

namespace collision
{
	inline bool LineIntersection2D(vector2df   A,
		vector2df   B,
		vector2df   C, 
		vector2df   D,
		irr::f32&     dist,
		vector2df&  point)
	{

		irr::f32 rTop = (A.Y-C.Y)*(D.X-C.X)-(A.X-C.X)*(D.Y-C.Y);
		irr::f32 rBot = (B.X-A.X)*(D.Y-C.Y)-(B.Y-A.Y)*(D.X-C.X);

		irr::f32 sTop = (A.Y-C.Y)*(B.X-A.X)-(A.X-C.X)*(B.Y-A.Y);
		irr::f32 sBot = (B.X-A.X)*(D.Y-C.Y)-(B.Y-A.Y)*(D.X-C.X);

		if ( (rBot == 0) || (sBot == 0))
		{
			//lines are parallel
			return false;
		}

		irr::f32 r = rTop/rBot;
		irr::f32 s = sTop/sBot;

		if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
		{
			dist = Vec2Distance(A,B) * r;

			point = A + r * (B - A);

			return true;
		}

		else
		{
			dist = 0;

			return false;
		}
	}

	inline bool LineIntersection2D(vector2df A,
		vector2df B,
		vector2df C, 
		vector2df D)
	{
		irr::f32 rTop = (A.Y-C.Y)*(D.X-C.X)-(A.X-C.X)*(D.Y-C.Y);
		irr::f32 sTop = (A.Y-C.Y)*(B.X-A.X)-(A.X-C.X)*(B.Y-A.Y);

		irr::f32 Bot = (B.X-A.X)*(D.Y-C.Y)-(B.Y-A.Y)*(D.X-C.X);

		if (Bot == 0)//parallel
		{
			return false;
		}

		irr::f32 invBot = 1.0f/Bot;
		irr::f32 r = rTop * invBot;
		irr::f32 s = sTop * invBot;

		if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
		{
			//lines intersect
			return true;
		}

		//lines do not intersect
		return false;
	}

	inline bool CircleCircleCollide(vector2df pos1, float radius1, vector2df pos2, float radius2)
	{
		float dis = Vec2Distance(pos1, pos2);
		float newrad = radius1 + radius2;

		if(dis < newrad)
		{
			return true;
		}

		return false;
	}

	static void Create4WallsFrombox(aabbox3df box) // big nasty looking function that does the trick!
	{
		vector3df a = vector3df(box.MinEdge.X, 0.0f, box.MaxEdge.Z);
		vector3df b = vector3df(box.MaxEdge.X, 0.0f, box.MaxEdge.Z);
		vector3df c = vector3df(box.MaxEdge.X, 0.0f, box.MinEdge.Z);
		vector3df d = vector3df(box.MinEdge.X, 0.0f, box.MinEdge.Z);

		vector3df n_a = a.crossProduct(b);
		vector3df n_b = b.crossProduct(c);
		vector3df n_c = c.crossProduct(d);
		vector3df n_d = d.crossProduct(a);

		n_a.normalize();
		n_b.normalize();
		n_c.normalize();
		n_d.normalize();

		Wall* wall1 = new Wall(vector2df(a.X, a.Z), vector2df(b.X, b.Z), vector2df(n_a.X, n_a.Y));
		Wall* wall2 = new Wall(vector2df(b.X, b.Z), vector2df(c.X, c.Z), vector2df(n_b.X, n_b.Y));
		Wall* wall3 = new Wall(vector2df(c.X, c.Z), vector2df(d.X, d.Z), vector2df(n_c.X, n_c.Y));
		Wall* wall4 = new Wall(vector2df(d.X, d.Z), vector2df(a.X, a.Z), vector2df(n_d.X, n_d.Y));

		TheGame::Instance()->GetWalls().push_back(wall1);
		TheGame::Instance()->GetWalls().push_back(wall2);
		TheGame::Instance()->GetWalls().push_back(wall3);
		TheGame::Instance()->GetWalls().push_back(wall4);
	}
}

#endif