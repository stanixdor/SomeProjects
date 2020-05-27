// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		StateMachine.h
//	Purpose:		use state machine for the demo implementation:
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

#include "Engine/GameObject/Component.h"
#include "Engine/Platform/Time/FrameRateController.h"

#include <vector>	//std::vector
#include <string>	//std::string

// forward declare the GameObject class
// This is so that we don't have to include it in the header
// remember that this is only valid if we reference that struct
// by pointer.

class GameObject;
class StateMachine;

class State
{
public:

	//SERIALIZABLE;
	State(const char * name);
	State() {}
	virtual ~State() {}

	StateMachine * mOwnerStateMachine;
	GameObject * mActor;
	std::string mName;
	f64 mTimeInState;

	

	virtual void initialize();

	virtual void Enter() {}
	virtual void Exit() {}
	virtual void Update() {}

	// internal 
	virtual void InternalEnter();
	virtual void InternalExit();
	virtual void InternalUpdate();

};


class StateMachine : public Component
{
public:

	SERIALIZABLE;

	// state container
	std::vector<State*> mStates;
	State * mCurrentState;
	State * mNextState;
	State * mInitialState;
	State * mPrevState;

	virtual void Initialize();
	StateMachine();
	virtual ~StateMachine();

	// Container management:

	template <typename T>
	State* AddState();

	template <typename T>
	void RemoveState();

	template<typename T>
	T* GetState();


	void Clear();

	// Set initial state
	void SetInitState(State * state);

	template <typename T>
	void SetInitState();
	template <typename T>
	void ChangeState();

	// Update 
	void Update();
	void Reset();

};


class SuperState : public State
{

	StateMachine mInternalStateMachine;

	SuperState(const char * name) :State(name) {}

	// internal 
	virtual void InternalEnter();
	virtual void InternalExit();
	virtual void InternalUpdate();
};



// Container management template :


//! ---------------------------------------------------------------------------
// \fn		GetState
// \brief	Finds a state by name
// \return	A pointer to the state if name matches, NULL otherwise.
// ---------------------------------------------------------------------------
template<typename T>
T* StateMachine::GetState()
{
	// Look for state of type T
	const auto it = std::find_if(mStates.begin(), mStates.end(), [](State * state)
	{
		return typeid(*state) == typeid(T);
	});
	// If found return it
	if (it != mStates.end())
		return static_cast<T *>(*it);
	// Otherwise return null
	else
		return nullptr;
}


//! ---------------------------------------------------------------------------
// \fn		SetInitState
// \brief	Sets the initial state to one state that whose name match
// ----------------------------------------------------------------------------
template <typename T>
void StateMachine::SetInitState()
{
	mInitialState = GetState<T>();
	mNextState = mInitialState;
}


//! ---------------------------------------------------------------------------
// \fn		ChangeState
// \brief	Changes state to the specified state. 
// ----------------------------------------------------------------------------
template <typename T>
void StateMachine::ChangeState()
{
	mNextState = GetState<T>();

	mPrevState = mCurrentState;
}


//! ---------------------------------------------------------------------------
// \fn		AddState
// \brief	Adds a new state to the state machine.
// ---------------------------------------------------------------------------
template <typename T>
State* StateMachine::AddState()
{
	if (GetState<T>() == nullptr)
	{
		State * state = new T;
		state->mOwnerStateMachine = this;
		state->mActor = m_GameObject;
		mStates.push_back(state);
		state->initialize();

		return state;
	}

	return GetState<T>();
}



//! ---------------------------------------------------------------------------
// \fn		RemoveState
// \brief	Removes a State from the state machine.
// ---------------------------------------------------------------------------
template <typename T>
void StateMachine::RemoveState()
{
	// Look for state of type T
	const auto it = std::find(mStates.begin(), mStates.end(), [](State * state)
	{
		return typeid(*state) == typeid(T);
	});
	if (it != mStates.end())
	{
		delete *it;
		mStates.erase(it);
	}
}

