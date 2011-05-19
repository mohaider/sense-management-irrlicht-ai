#ifndef REGIONAL_SENSE_MANAGER_H
#define REGIONAL_SENSE_MANAGER_H

#include "Singleton.h"
#include "Sensor.h"
#include <list>
#include <queue>

class RegionalSenseManager
{
public:

	struct Notification 
	{
		float time;
		Sensor* m_sensor;
		Signal* m_signal;
	};

	struct LessThan
	{
		bool operator() (Notification* n1, Notification* n2);
	};

	~RegionalSenseManager() {}

	// the notification queue
	std::priority_queue<Notification*, std::vector<Notification*>, LessThan> m_pQueue;

	void AddSignal(Signal* signal);
	void AddSensor(Sensor* sensor);
	void SendSignals();

	// Emit signals using these functions, I couldn't decide a good place for them so placed them in here
	void EmitVisualSignal(float strength, vector3df position, signal_origin origin);
	void EmitSoundSignal(float strength, vector3df position, signal_origin origin);

private:

	RegionalSenseManager() {}
	friend class Singleton<RegionalSenseManager>;

	// the list of available sensors
	std::list<Sensor*> m_sensorList;
};

typedef Singleton<RegionalSenseManager> TheRSM;

#endif