#include "RegionalSenseManager.h"
#include "PrecisionTimer.h"
#include "Signal.h"
#include "SoundModality.h"
#include "VisionModality.h"

bool RegionalSenseManager::LessThan::operator () (Notification* n1, Notification* n2)
{
	return (n1->time < n2->time);
}

void RegionalSenseManager::AddSignal(Signal* signal)
{
	for(std::list<Sensor*>::iterator it = m_sensorList.begin(); it != m_sensorList.end(); ++it)
	{
		// check the modality first
		if(!(*it)->DetectsModality((signal->m_modality)))
		{
			continue;
		}

		// Find the distance
		float distance = signal->m_position.getDistanceFrom((*it)->m_position);
		// Check range
		if(signal->m_modality->m_maxRange < distance)
		{
			continue;
		}

		// Find signal intensity
		float intensity = signal->m_strength * pow(signal->m_modality->m_attenuation, distance);
		// Check threshold
		if(intensity < (*it)->m_threshold)
		{
			continue;
		}

		// Perform modality specific checks
		if(!signal->m_modality->ExtraChecks(signal, (*it)))
		{
			continue;
		}

   		float time = (float)TheTimer::Instance()->CurrentTime() + distance * signal->m_modality->m_inverseTransmissionSpeed;

		Notification* newNotification = new Notification();
		newNotification->time = time;
		newNotification->m_sensor = (*it);
		newNotification->m_signal = signal;
		m_pQueue.push(newNotification);

		SendSignals();
	}
}

void RegionalSenseManager::SendSignals()
{
	float currentTime = (float)TheTimer::Instance()->CurrentTime();

	while(!m_pQueue.empty())
	{
   		Notification* notification = m_pQueue.top();

        if(notification->time >= currentTime)
		{
			notification->m_sensor->Notify(*notification->m_signal);
			m_pQueue.pop();
		}
		else
		{
			break;
		}
	}
}

void RegionalSenseManager::AddSensor(Sensor* sensor)
{
	m_sensorList.push_back(sensor);
}

void RegionalSenseManager::EmitSoundSignal(float strength, vector3df position, signal_origin origin)
{
	Signal newSignal;
	newSignal.m_modality = TheSoundModality::Instance();
	newSignal.m_position = position;
	newSignal.m_strength = strength;
	newSignal.m_origin = origin;
	AddSignal(&newSignal);

}

void RegionalSenseManager::EmitVisualSignal(float strength, vector3df position, signal_origin origin)
{
	Signal newSignal;
	newSignal.m_modality = TheVisionModality::Instance();
	newSignal.m_position = position;
	newSignal.m_strength = strength;
	newSignal.m_origin = origin;
	AddSignal(&newSignal);
}