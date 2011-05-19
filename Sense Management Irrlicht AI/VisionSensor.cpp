#include "VisionSensor.h"
#include "MessageSender.h"
#include "MessageTypes.h"
#include "VisionModality.h"

bool VisionSensor::DetectsModality(Modality* modality)
{
	if(modality == TheVisionModality::Instance())
	{
		return true;
	}

	return false;
}

void VisionSensor::Notify(const Signal& signal)
{
	if(signal.m_origin == origin_player) // if the signal originates from the player then send a spotted player message
	{
		TheMessageSender::Instance()->SendMessage(0.05, m_pOwner->GetAIID(), m_pOwner->GetAIID(), Msg_SpottedEnemy, signal);
	}
}