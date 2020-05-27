// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:		EnemyStateMachine.cpp
//	Purpose:		use state machine for the demo sloth implementation:
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "Engine/Logic/StateMachine/EnemyStateMachine.h"
#include "GameObject\GameObject.h"
#include "Physics\RigidBody.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"

#include "Engine\Platform\EventSystem\type_info.h"
#include "Engine\Platform\EventSystem\event.h"
#include "Game\Components\DetectorComponent.h"



//data for the collisions
//OnCollisionStarted CollisionEvent;
//const OnCollisionPersisted CollisionEvent;
//const OnCollisionEnded CollisionEvent;

static GameObject * Enemy;



//---------------------------------------------------------------------------/
// IDLE STATE																 /
//---------------------------------------------------------------------------/

void Enemy_Idle_State::Enter()
{
	std::cout << "Entered Enemy_Idle" << std::endl;
}


void Enemy_Idle_State::Exit()
{

}

void Enemy_Idle_State::Update()
{

	
	//if(Detector::collisionStarted())
	//timer time;

	
	
	/*timer time;
	int i = 0;
	while (time.startedAt == 5 * i)
	{
		++i;
		time.Start();
		if( time.startedAt ==  10)
			std::cout << "hola" << std::endl;
	}
	time.Reset();*/
		
	
}

void Enemy_Idle_State::Initialize()
{
	/*using namespace Messaging;

	Enemy = mOwnerStateMachine->GetOwner();

	// register to the event:

	EventHandler* temp = &mOwnerStateMachine->GetOwner()->GetEventHandler();


	mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &Enemy_Idle_State::OnPlayerDetected);

	//EventHandler* temp = &Enemy->GetEventHandler();
	//Enemy->GetEventHandler().register_handler(this, &Enemy_Idle_State::OnPlayerDetected);

	// subscribe this to this type of event
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnPlayerDetected>());*/
	
}

void Enemy_Idle_State::OnPlayerDetected()
{

}


//---------------------------------------------------------------------------/
// WARNING STATE														     /
//---------------------------------------------------------------------------/

void Enemy_Warning_State::Enter()
{
	std::cout << "warning" << std::endl;
}


void Enemy_Warning_State::Exit()
{

}

void Enemy_Warning_State::Update()
{

}


//---------------------------------------------------------------------------/
// DETECT STATE																 /
//---------------------------------------------------------------------------/


void Enemy_Detect_State::Enter()
{
	std::cout << "Entered Detect" << std::endl;
}


void Enemy_Detect_State::Exit()
{

}

void Enemy_Detect_State::Update()
{

	
}

void Enemy_Detect_State::OnPlayerDetected()
{
}

void Enemy_Detect_State::Initialize()
{
}
