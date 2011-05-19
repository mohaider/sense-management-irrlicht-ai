#ifndef SIGNAL_H
#define SIGNAL_H

/*
	Interface class for signals

*/

#include "Modality.h"
#include "IrrlichtObject.h"

enum signal_origin // enum for signal origins
{
	origin_player,
	origin_footprint,
};

class Signal
{
public:

	Signal() : m_strength(0), m_position(vector3df()), m_modality(0), m_origin(origin_player) {}
	virtual ~Signal() {}

	float m_strength;
	vector3df m_position;
	Modality* m_modality;
	signal_origin m_origin; // used for how a sensor should react to different objects
};

#endif