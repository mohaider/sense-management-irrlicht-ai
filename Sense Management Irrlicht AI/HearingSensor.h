#ifndef HEARING_SENSOR_H
#define HEARING_SENSOR_H

#include "Sensor.h"

// Hearing Sensor

class HearingSensor : public Sensor
{
public:
	virtual bool DetectsModality(Modality* modality);
	virtual void Notify(const Signal& signal);
};

#endif