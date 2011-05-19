#ifndef MESSAGE_H
#define MESSAGE_H

#include <math.h>
#include "Signal.h"

/*
	Messages can be sent to any AIObject in the AIObject map
	They include the receiver, sender, time of dispatch and a signal (if needed)
*/

struct Message
{
	int Sender;
	int Receiver;
	int Msg;
	double DispatchTime;
	Signal signal;

	Message() : DispatchTime(-1), Sender(-1), Receiver(-1), Msg(-1) {}

	Message(double time,
		int    sender,
		int    receiver,
		int    msg,
		Signal  signal) : DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg),
		signal(signal) {}

	Message(double time,
		int    sender,
		int    receiver,
		int    msg) : DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg) {}
};

#endif