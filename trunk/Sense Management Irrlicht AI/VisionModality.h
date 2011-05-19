#ifndef VISION_MODALITY_H
#define VISION_MODALITY_H

#include "Modality.h"
#include "Singleton.h"

class VisionModality : public Modality
{
public:

	virtual void Load();
	virtual bool ExtraChecks(Signal* signal, Sensor* sensor);

private:

	VisionModality();
	friend class Singleton<VisionModality>;

	float m_fovAngle;
};

typedef Singleton<VisionModality> TheVisionModality;

#endif