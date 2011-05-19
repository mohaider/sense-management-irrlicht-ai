#ifndef SOUND_MODALITY_H
#define SOUND_MODALITY_H

#include "Modality.h"
#include "Singleton.h"

class SoundModality : public Modality
{
public:

	virtual void Load();
	virtual bool ExtraChecks(Signal* signal, Sensor* sensor);

private:

	SoundModality();
	friend class Singleton<SoundModality>;
};

typedef Singleton<SoundModality> TheSoundModality;

#endif