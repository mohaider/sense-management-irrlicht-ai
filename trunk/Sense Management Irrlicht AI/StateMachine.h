#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H


#include "State.h"
#include <assert.h>
#include <map>
#include <string>

struct Message;

template<class T>
class StateMachine
{
public:

	StateMachine(T* owner) : m_pOwner(owner), m_pCurrentState(NULL), m_pPreviousState(NULL), m_pGlobalState(NULL) {}

	void SetCurrentState(State<T> *state)
	{
		m_pCurrentState = state;
	}

	void SetGlobalState(State<T> *state)
	{
		m_pGlobalState = state;
	}

	void SetPreviousState(State<T> *state)
	{
		m_pPreviousState = state;
	}

	void Update() const
	{
		if( m_pGlobalState ) m_pGlobalState->Execute(m_pOwner);

		if( m_pCurrentState ) m_pCurrentState->Execute(m_pOwner);
	}

	void ChangeState(State<T>* state)
	{
		assert(state && "Trying to change to invalid state");

		m_pPreviousState = m_pCurrentState;

		m_pCurrentState->OnExit(m_pOwner);

		m_pCurrentState = state;

		m_pCurrentState->OnEnter(m_pOwner);
	}

	void RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}

	State<T>* GetCurrentState()
	{
		return m_pCurrentState;
	}

	State<T>* GetGlobalState()
	{
		return m_pGlobalState;
	}

	State<T>* GetPreviousState()
	{
		return m_pPreviousState;
	}

	bool IsInState(const State<T> &state) const
	{
		return typeid(*m_pCurrentState) == typeid(state);
	}

	bool HandleMessage(Message* msg) const
	{
		if(m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, *msg))
		{
			return true;
		}

		if(m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, *msg))
		{
			return true;
		}

		return false;
	}

private:

	T* m_pOwner;

	State<T>* m_pCurrentState;

	State<T>* m_pPreviousState;

	State<T>* m_pGlobalState;
};

#endif
