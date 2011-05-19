#ifndef VISION_SENSOR_H
#define VISION_SENSOR_H

#include "Sensor.h"

class VisionSensor : public Sensor
{
public:
	virtual bool DetectsModality(Modality* modality);
	virtual void Notify(const Signal& signal);
};

#endif