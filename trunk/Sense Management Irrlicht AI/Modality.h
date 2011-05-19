#ifndef MODALITY_H
#define MODALITY_H

/*
	Interface class for Modalities

	Extra checks is used for modality specific checks.
*/

class Sensor;
class Signal;

class Modality
{
public:

	Modality() : m_maxRange(0), m_attenuation(0), m_inverseTransmissionSpeed(0) {}
	virtual ~Modality() {}

	virtual void Load() = 0;
	virtual bool ExtraChecks(Signal* signal, Sensor* sensor) = 0;

	float m_maxRange;
	float m_attenuation;
	float m_inverseTransmissionSpeed;
};

#endif