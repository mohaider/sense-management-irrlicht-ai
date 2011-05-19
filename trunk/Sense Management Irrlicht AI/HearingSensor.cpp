#include "HearingSensor.h"
#include "SoundModality.h"
#include "MessageSender.h"
#include "MessageTypes.h"

bool HearingSensor::DetectsModality(Modality *modality)
{
	if(modality == TheSoundModality::Instance())
	{
		return true;
	}

	return false;
}

void HearingSensor::Notify(const Signal& signal)
{
	if(signal.m_origin == origin_player) // if the signal originates from the player then send a spotted player message
	{
		TheMessageSender::Instance()->SendMessage(1.0, m_pOwner->GetAIID(), m_pOwner->GetAIID(), Msg_HeardSomething, signal);
	}
}



