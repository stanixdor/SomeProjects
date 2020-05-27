// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		SlothStateMachine.cpp
//	Purpose:		
//	Author:			Javier Tafur
// ---------------------------------------------------------------------------
//#include "EnemiesStates.h"
#include "Game/Logic/StateMachines/SlothStateMachine.h"
#include "Physics\RigidBody.h"
#include "Levels\LevelTest.h"
#include "GLM\gtx\vector_angle.hpp"
#include "Engine\Platform\Resource Manager\ResourceManager.h"


// SLOTH ///

void Sloth_Idle_State::Initialize()
{
	
	mOwner = mOwnerStateMachine->GetOwner();
}

void Sloth_Idle_State::Enter()
{
}


void Sloth_Idle_State::Update()
{
}

void Sloth_Idle_State::Exit()
{
}

void Sloth_Detect_State::Initialize()
{
	mOwner = mOwnerStateMachine->GetOwner();
}


void Sloth_Detect_State::Enter()
{
}


void Sloth_Detect_State::Update()
{
}

void Sloth_Detect_State::Exit()
{
}

void Sloth_Warning_State::Initialize()
{
	mOwner = mOwnerStateMachine->GetOwner();
}

void Sloth_Warning_State::Enter()
{
}


void Sloth_Warning_State::Update()
{
}

void Sloth_Warning_State::Exit()
{
}


void SlothStateMachine::Initialize()
{


	m_name = "SlothStateMachine";
	AddState<Sloth_Idle_State>();		//States
	AddState<Sloth_Detect_State>();
	AddState<Sloth_Warning_State>();
	//initial state
	SetInitState<Sloth_Idle_State>();

	//data per state:

	GetState<Sloth_Idle_State>()->CompColumns = 1;
	GetState<Sloth_Idle_State>()->CompRows = 1;
	GetState<Sloth_Idle_State>()->CompSprites = 1;
	GetState<Sloth_Idle_State>()->CompDuration = 1;
	GetState<Sloth_Idle_State>()->CompDelays.resize(1);
	GetState<Sloth_Idle_State>()->StateTexture = ResourceManager::textures["perezoso first.png"].get();
	GetState<Sloth_Idle_State>()->mAnimSpeed = 0.2f;
	for (auto& delay : GetState<Sloth_Idle_State>()->CompDelays)
		delay = 999.f;

	GetState<Sloth_Warning_State>()->CompColumns = 18;
	GetState<Sloth_Warning_State>()->CompRows = 1;
	GetState<Sloth_Warning_State>()->CompSprites = 18;
	GetState<Sloth_Warning_State>()->CompDuration = 1;
	GetState<Sloth_Warning_State>()->CompDelays.resize(18);
	GetState<Sloth_Warning_State>()->StateTexture = ResourceManager::textures["perezoso surprise.png"].get();
	GetState<Sloth_Warning_State>()->mAnimSpeed = 1.0f;
	for (auto& delay : GetState<Sloth_Warning_State>()->CompDelays)
		delay = 0.042f;

	GetState<Sloth_Detect_State>()->CompColumns = 18;
	GetState<Sloth_Detect_State>()->CompRows = 1;
	GetState<Sloth_Detect_State>()->CompSprites = 18;
	GetState<Sloth_Detect_State>()->CompDuration = 1;
	GetState<Sloth_Detect_State>()->CompDelays.resize(18);
	GetState<Sloth_Detect_State>()->StateTexture = ResourceManager::textures["perezoso surprise.png"].get();
	GetState<Sloth_Detect_State>()->mAnimSpeed = 1.0f;
	for (auto& delay : GetState<Sloth_Detect_State>()->CompDelays)
		delay = 0.042f;
}