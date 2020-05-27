// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    DetectorComponent.cpp 
//	Purpose:		use for the detection of the enemies over the player,
//  applies, collision detection and state machines.
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "Game\Components\DetectorComponent.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Events\OnPlayerDetected.h"
#include "Platform\Time\FrameRateController.h"
#include "EngineDebug\ConsolePrinting.h"
#include "Engine/Graphics/GraphicsSystem.h"
#include "Engine/Platform/Resource Manager/ResourceManager.h"
//#include "Game\Logic/StateMachines\EnemiesStates.h"
#include "Game\Logic\StateMachines\SlothStateMachine.h"
#include "Components\Checkpoint.h"



REFLECTION(Detector, pathDetector);
//! ---------------------------------------------------------------------------
// \fn	Collisions functions for the detector
// ---------------------------------------------------------------------------
void Detector::collisionStarted(const OnCollisionStarted& mEvent)
{
	//loquesea.target = pointer;
	if (mEvent.mData.obj1->getCollisionGroup().mName == "player" || mEvent.mData.obj2->getCollisionGroup().mName == "player")
	{
		if (activated)
			objectManager.FindObjectByName("player")->m_transform->SetPos(Checkpoint::respawnPosition);

	}
}

void Detector::collisionPersisted(const OnCollisionPersisted& mEvent)
{
	//loquesea.target = pointer;
	if (mEvent.mData.obj1->getCollisionGroup().mName == "player" || mEvent.mData.obj2->getCollisionGroup().mName == "player")
	{
		if (activated)
			objectManager.FindObjectByName("player")->m_transform->SetPos(Checkpoint::respawnPosition);

	}
}

void Detector::Initialize()
{
	////register object and function
	using namespace Messaging;

	// register to the event:

	timeToActivate = 4.f;
	EventHandler* temp = &GetEventHandler();
	temp->register_handler(this, &Detector::collisionStarted);
	temp->register_handler(this, &Detector::collisionPersisted);

	// subscribe this to this type of event
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	timer.Start();
	deactivate();
}

void Detector::Update()
{

	if (activated == false && timer.GetTimeSinceStart() >= timeToActivate)
	{
		timer.Reset();
		activate();
		const auto target = pathDetector.FindObject();
		if (target != nullptr)
			target->FindComponentByType<SlothStateMachine>()->ChangeState<Sloth_Detect_State>();
	}
	if (activated == true && timer.GetTimeSinceStart() >= timeToActivate)
	{
		timer.Reset();
		deactivate();
		const auto target = pathDetector.FindObject();
		if (target != nullptr)
			target->FindComponentByType<SlothStateMachine>()->ChangeState<Sloth_Idle_State>();
	}
}

void Detector::activate()
{
	activated = true;
	m_GameObject->FindComponentByType<Graphics::Renderable>()->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));

}

void Detector::deactivate()
{
	activated = false;
	m_GameObject->FindComponentByType<Graphics::Renderable>()->setColor(glm::vec4(201.f, 159.f, 10.f, 0.2f));

}
