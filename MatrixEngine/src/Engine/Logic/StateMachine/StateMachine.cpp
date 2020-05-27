// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		StateMachine.cpp
//	Purpose:		use state machine for the demo implementation:
//	Author:			Javier Tafur, javier.tafur
// ----------------------------------------------------------------------------

#include "StateMachine.h"


// ----------------------------------------------------------------------------
#pragma region// STATE MACHINE

void StateMachine::Initialize()
{
}

//! ---------------------------------------------------------------------------
// \fn		Constructor
// \brief	Sets all state machine values to NULL (note: they are all pointers).
// ---------------------------------------------------------------------------
StateMachine::StateMachine()
{
	// state = null
	m_name = "State Machine";
	mCurrentState = nullptr;
	mNextState = nullptr;
	mInitialState = nullptr;
}


//! ---------------------------------------------------------------------------
// \fn		Destructor
// \brief	Clears all the states.
// \details	You should only call Clear() and avoid redundant call
// ---------------------------------------------------------------------------
StateMachine::~StateMachine()
{
	Clear();
}


//! ---------------------------------------------------------------------------
// \fn		Clear
// \brief	Removes all the states from the state machine and sets the 
//			state pointers (current, next, initial) to NULL.
//
// \details	Remember to delete the memory for the states. 
// ---------------------------------------------------------------------------
void StateMachine::Clear()
{
	for (State * state : mStates)
		delete state;
	mStates.clear();

	mCurrentState = nullptr;
	mNextState = nullptr;
	mInitialState = nullptr;
}


//! ---------------------------------------------------------------------------
// \fn		SetInitState
// \brief	Sets the initial state to the given pointer.
//
// \details	You only modify mInitialState IF AND ONLY IF 'state'
//			exists in the mStates vector. (i.e., it's been added to state 
//			machine prior to calling this function. 
// ---------------------------------------------------------------------------
void StateMachine::SetInitState(State * state)
{
	//the initial state to the given state:


	for (std::vector<State*>::iterator it = mStates.begin(); it != mStates.end(); ++it)
	{
		if (state == (*it))
		{
			mInitialState = state;
		}
	}

}

//! ---------------------------------------------------------------------------
// \fn		Update
// \brief	Update of the state machine. Meant to be called every frame.
//
// \details	
//			- The function should call only call the "Internal...()" functions
//				on the current state.
//			- This function also performs the state change. This happens when
//				mCurrentState != mNextState.
//			- when changing state, you must call InternalExit and InternalEnter
//				on the old state and new state, respectively.
// ---------------------------------------------------------------------------
void StateMachine::Update()
{
	//if (mCurrentState == nullptr)
	//{
	//	mCurrentState = mNextState;
	//	mNextState->InternalEnter();
	//
	//}
	if (mCurrentState != mNextState)
	{
		//SANITY CHECK CASE IF  mCurrent is null:
		if (mCurrentState)
			mCurrentState->InternalExit();
		mCurrentState = mNextState;
		mNextState->InternalEnter();

	}
	if (mCurrentState)
		mCurrentState->Update();
}


//! ---------------------------------------------------------------------------
// \fn		Reset
// \brief	Resets the state machine. 
//
// \details	This is equivalent to calling ChangeState(mInitialState);
// ---------------------------------------------------------------------------
void StateMachine::Reset()
{
	mNextState = mInitialState;
}


//! ---------------------------------------------------------------------------
// \fn		Serialization functions 
// \brief	Resets the state machine. 
//
// \details	This is equivalent to calling ChangeState(mInitialState);
// ---------------------------------------------------------------------------
void StateMachine::to_json(Json::Value &) const
{}

void StateMachine::from_json(const Json::Value &)
{}


#pragma endregion

// ----------------------------------------------------------------------------
#pragma region// STATE

void State::initialize()
{
}

//! ---------------------------------------------------------------------------
// \fn		Custom Constructor
// \brief	Sets the default values of the state.
//
// \details:
//				- mName is set to the specified name
//				- all other values are set to NULL. 
// 
// \return	
// ---------------------------------------------------------------------------
State::State(const char * name)
{
	//set default values:
	mName = name;
	mOwnerStateMachine = nullptr;
	mActor = nullptr;
}


//! ---------------------------------------------------------------------------
// \fn		InternalEnter
// \brief	Called by the state machine when switchin to this state.
//
// \details	
//			- Sets mTimeState = 0.0f
//			- Calls Enter(). 
// ---------------------------------------------------------------------------
void State::InternalEnter()
{
	mTimeInState = 0.0f;
	Enter();
}

//! ---------------------------------------------------------------------------
// \fn		InternalExit
// \brief	Called by the state machine when switchin out of this state.
//
// \details: Only calls Exit().
// ---------------------------------------------------------------------------
void State::InternalExit()
{
	Exit();
}

//! ---------------------------------------------------------------------------
// \fn		InternalUpdate
// \brief	Called by the state machine while it's in this state.
//
// \details:
//				- Increments mTimeInState by gAEFrameTime
//				- Calls Update();
// 
// \return	
// ---------------------------------------------------------------------------
void State::InternalUpdate()
{
	mTimeInState += FRC::FrameRateController::Instance()->GetFrameTime();
	Update();
}

#pragma endregion

// ----------------------------------------------------------------------------
#pragma region// SUPER STATE (STATE AS A STATE MACHINE)


//! ---------------------------------------------------------------------------
// \fn		InternalEnter
// \brief	Resets the internal state machine and calls InternalEnter
//
// \details: 
//			- Before doing anything, you should set the mInternalStateMachine.mActor
//			equal to this state's mActor variable. You must do the same for EVERY
//			state in the mInternalStateMachine.mStates vector.
//			- the internal state machine must be reset.
//			- Don't forget to call State::InternalEnter to inherit the default
//			behavior of the state. (this is also what calls Enter()). 
// ---------------------------------------------------------------------------
void SuperState::InternalEnter()
{
	//mInternalStateMachine.mActor = this->mActor;
	//do the same for EVERY
	//state in the mInternalStateMachine.mStates vector
	for (std::vector<State*>::iterator it = mInternalStateMachine.mStates.begin(); it != mInternalStateMachine.mStates.end(); ++it)
	{
		mInternalStateMachine.m_GameObject = this->mActor;
		mInternalStateMachine.Reset();
	}
	State::InternalEnter();

}


//! ---------------------------------------------------------------------------
// \fn		InternalExit
// \brief	same behavior as parent State struct.
//
// \details:	Only calls State::InternalExit().
// \note:		This is technically redundant and this function should not be
//				overriden (it calls the base function). It's defined here to 
//				avoid potential confusions when doing the assignment.
// ---------------------------------------------------------------------------
void SuperState::InternalExit()
{
	State::InternalExit();
}


//! ---------------------------------------------------------------------------
// \fn		InternalUpdate
// \brief	Updates the internal state machine first, then calls State::InternalUpdate()
// ---------------------------------------------------------------------------
void SuperState::InternalUpdate()
{
	// update internal state machine:
	for (std::vector<State*>::iterator it = mInternalStateMachine.mStates.begin(); it != mInternalStateMachine.mStates.end(); ++it)
	{
		// Update the internal state machine:
		if (mInternalStateMachine.mCurrentState != mInternalStateMachine.mNextState)
		{
			mInternalStateMachine.Update();
		}
	}

	State::InternalUpdate();
}

#pragma endregion