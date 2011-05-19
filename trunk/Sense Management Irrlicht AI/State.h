#ifndef STATE_H
#define STATE_H

struct Message;

template <class T>
class State
{
public:

	virtual ~State() {}

	// execute the states actions
	virtual void Execute(T* t) = 0;

	// called when the state is first called
	virtual void OnEnter(T* t) = 0; 

	// called when the state is exited
	virtual void OnExit(T* t) = 0; 

	virtual bool OnMessage(T* t, const Message& msg) = 0;
};

#endif