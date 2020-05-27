// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		EnemyStateMachine.h
//	Purpose:		use state machine for the demo sloth implementation:
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#pragma once

//state machine component
#include "Engine/Logic/StateMachine/StateMachine.h"
#include "Engine/Platform/Input/Input.h"
#include "Physics\CollisionEvents\OnCollisionEvent.h"
#include "Logic\StateMachine\AnimationStateMachine.h"
#include "Events\OnPlayerDetected.h"
#include <stdio.h>


// STATES:

class Enemy_Idle_State : public AnimState
{
public:
	void Enter();
	void Exit();
	void Update();

	void Initialize();

	void OnPlayerDetected();

};


class Enemy_Warning_State : public AnimState
{
public:
	void Enter();
	void Exit();
	void Update();

	//void Initialize();

	//void OnPlayerDetected();

};


class Enemy_Detect_State : public AnimState
{
public:
	void Enter();
	void Exit();
	void Update();

	void OnPlayerDetected();

	void Initialize();

};
