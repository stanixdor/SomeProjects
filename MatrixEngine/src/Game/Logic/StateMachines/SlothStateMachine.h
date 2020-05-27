// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		SlothStateMachine.h
//	Purpose:		
//	Author:			Javier Tafur
// ---------------------------------------------------------------------------

#pragma once

#include "Logic\StateMachine\AnimationStateMachine.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"





//									SLOTH

class SlothStateMachine : public StateMachine
{
public:
	SERIALIZABLE_NO_PROPERTIES
	void Initialize();
};

class Sloth_Idle_State : public AnimState
{
public:
	void Enter();
	void Exit();
	void Update();

	virtual void Initialize();

	//void OnPlayerDetected();

};


class Sloth_Warning_State : public AnimState
{
public:
	void Enter();
	void Exit();
	void Update();

	virtual void Initialize();

	//void OnPlayerDetected();

};


class Sloth_Detect_State : public AnimState
{
public:
	void Enter();
	void Exit();
	void Update();

	//virtual void OnPlayerDetected();

	virtual void Initialize();

};


