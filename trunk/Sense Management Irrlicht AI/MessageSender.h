#ifndef MESSAGE_SENDER_H
#define MESSAGE_SENDER_H

#include <queue>
#include <iostream>
#include "Singleton.h"
#include "Message.h"

class AIObject;

class MessageSender
{
public:

	void SendMessage(double delay, int sender, int receiver, int msg, Signal signal);

	// send message without a signal
	void SendMessageNS(double delay, int sender, int receiver, int msg);

	void SendDelayedMessages();

	struct LessThan
	{
		bool operator()(Message* a,Message* b);
	};

private:

	MessageSender() {}
	friend class Singleton<MessageSender>;

	void Discharge(AIObject* pReceiver, Message* msg);

	typedef std::priority_queue<Message*, std::vector<Message*>, LessThan> Q;
	Q m_messageQueue;
};

typedef Singleton<MessageSender> TheMessageSender;
#endif