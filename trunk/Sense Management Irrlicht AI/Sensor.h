#ifndef SENSOR_H
#define SENSOR_H

/*
	Interface class for sensors

	Detects Modality - Check if this sensor can sense the passed modality
*/

#include "Modality.h"
#include "Signal.h"
#include "IrrlichtObject.h"
#include "AIObject.h"
#include <list>

class Sensor
{
public:

	Sensor() : m_position(0), m_orientation(0), m_threshold(0), m_pOwner(0) {}

	virtual bool DetectsModality(Modality* modality) = 0;
	virtual void Notify(const Signal& signal) = 0;

	vector3df m_position;
	float m_orientation;
	float m_threshold;

	AIObject* m_pOwner;
};

#endif