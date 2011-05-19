#include "MessageSender.h"
#include "AIObject.h"
#include "Game.h"
#include "PrecisionTimer.h"

#undef SendMessage // <---- Worst error ever when this isn't here, took ages!!!!

bool MessageSender::LessThan::operator()(Message* a,Message* b)
{
	return (a->DispatchTime < b->DispatchTime);
}

void MessageSender::Discharge(AIObject* pReceiver, Message* msg)
{
	if(!pReceiver->HandleMessage(msg))
	{
		std::cout << "Message not handled";
	}
}

void MessageSender::SendMessage(double delay, int sender, int receiver, int msg, Signal signal)
{
	AIObject* pSender = TheGame::Instance()->GetAIObject(sender);
	AIObject* pReceiver = TheGame::Instance()->GetAIObject(receiver);

	if(pReceiver == NULL)
	{
		std:: cout << "No receiver with this ID\n";

		return;
	}

	Message* message = new Message(delay, sender, receiver, msg, signal);

	if(delay <= 0.0f)
	{
		message->signal = signal;
		Discharge(pReceiver, message);
	}
	else
	{
		double CurrentTime = TheTimer::Instance()->CurrentTime();
		message->DispatchTime = CurrentTime + delay;
		message->signal = signal;

		m_messageQueue.push(message);
	}
}

void MessageSender::SendMessageNS(double delay, int sender, int receiver, int msg)
{
	AIObject* pSender = TheGame::Instance()->GetAIObject(sender);
	AIObject* pReceiver = TheGame::Instance()->GetAIObject(receiver);

	if(pReceiver == NULL)
	{
		std:: cout << "No receiver with this ID\n";

		return;
	}

	Message* message = new Message(delay, sender, receiver, msg);

	if(delay <= 0.0f)
	{
		Discharge(pReceiver, message);
	}
	else
	{
		double CurrentTime = TheTimer::Instance()->CurrentTime();
		message->DispatchTime = CurrentTime + delay;
		m_messageQueue.push(message);
	}
}

void MessageSender::SendDelayedMessages()
{
	double CurrentTime = TheTimer::Instance()->CurrentTime();

	while(!m_messageQueue.empty())
	{
		Message* msg = m_messageQueue.top();

		AIObject* pReceiver = TheGame::Instance()->GetAIObject(msg->Receiver);

		if(msg->DispatchTime >= CurrentTime)
		{
			Discharge(pReceiver, msg);

			m_messageQueue.pop();
		}
		else
		{
			break;
		}
	}
}