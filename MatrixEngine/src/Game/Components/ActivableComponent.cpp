// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    ActivableComponent.cpp 
//	Purpose:		use for the activation of level, buttons etc.
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "Game/Components/ActivableComponent.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Events\OnPlayerDetected.h"
#include "Engine/Logic/StateMachine/EnemyStateMachine.h"
#include "EngineDebug\ConsolePrinting.h"
#include "Engine/Graphics/GraphicsSystem.h"
#include "Engine/Platform/Resource Manager/ResourceManager.h"
#include "Utils\CogPath.h"
#include "Components\ActivObject.h"

REFLECTION(Activable, m_activableCogPaths, mbUseTimer, mbTimerRepeat, mTimerDuration);


//std::vector<ActivObject*> Activable::m_activableObjects;

//! ---------------------------------------------------------------------------
// \fn	Collisions functions for the activable
// ---------------------------------------------------------------------------

void Activable::collisionStarted(const OnCollisionStarted& mEvent)
{
	
	if ((mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("TongueCollisionGroup") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("TongueCollisionGroup")) && (timer.GetTimeSinceStart() > 2.0f))
	{
		m_rend->pTexture = ResourceManager::textures["palanca1.png"].get();
		for (auto & active_objects : m_activableCogPaths)
		{
			//active_objects.FindObject()->FindComponentByType<ActivObject>()->SetCurrentActive(false);
			active_objects.FindObject()->FindComponentByType<ActivObject>()->ToggleActivate();
		}
	}
	if ((mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("FakeTongueCollisionGroup") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("FakeTongueCollisionGroup")) && (timer.GetTimeSinceStart() > 2.0f))
	{
		m_rend->pTexture = ResourceManager::textures["palanca1.png"].get();
		for (auto & active_objects : m_activableCogPaths)
		{
			//active_objects.FindObject()->FindComponentByType<ActivObject>()->SetCurrentActive(false);
			active_objects.FindObject()->FindComponentByType<ActivObject>()->ToggleActivate();
		}
	}

	if (mbUseTimer)
	{
		mbTimerActive = true;
		timer.Reset();
	}
	
}


void Activable::collisionPersisted(const OnCollisionPersisted&)
{
	/*using namespace Messaging;
	Messaging::EventHandler* temp = &mDetectorOwner->GetEventHandler();
	Messaging::EventDispatcher::get_instance().trigger_event_for_handler(*temp, OnPlayerDetected());*/
	//std::cout << "is activated" << std::endl;

	/*if (mEvent.mData.obj1->getCollisionGroup().mName == "player" || mEvent.mData.obj2->getCollisionGroup().mName == "player")
	{
		if (Input::Instance()->KeyTriggered('M') && (timer.GetTimeSinceStart() > 2.0f))
		{
			m_GameObject->FindComponentByType<Graphics::Renderable>()->pTexture = ResourceManager::textures["Palanca on.png"].get();
			for (auto & active_objects : m_activableObjects)
				active_objects->is_active = false;
		}
			

	}*/

}

void Activable::collisionEnded(const OnCollisionEnded&)
{
}


void Activable::Initialize()
{

	m_rend = m_GameObject->FindComponentByType<Graphics::Renderable>();

	////register object and function
	using namespace Messaging;

	// register to the event:

	EventHandler* temp = &GetEventHandler();
	temp->register_handler(this, &Activable::collisionStarted);
	temp->register_handler(this, &Activable::collisionPersisted);
	temp->register_handler(this, &Activable::collisionEnded);

	// subscribe this to this type of event
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
	
	// for each cog path, retrieve the ActiveObject component and add to the m_activableObject array
	m_activableObjects.clear();
	for (auto & cp : m_activableCogPaths)
	{
		//m_activableObjects.push_back(cp.target);
		const auto target = cp.FindObject();
		if (target != nullptr)
			m_activableObjects.push_back(target);
	}
	
}


void Activable::Update()
{
	if (mbUseTimer) 
	{
		if (timer.GetTimeSinceStart() <= 0)
		{
			timer.Start();
		}

		else if (timer.GetTimeSinceStart() >= mTimerDuration)
		{
			if (mbTimerRepeat)
				timer.Reset();
			else
				mbTimerActive = false;

			m_rend->pTexture = ResourceManager::textures["palanca2.png"].get();
			for (auto & active_objects : m_activableCogPaths)
			{
				active_objects.FindObject()->FindComponentByType<ActivObject>()->ToggleActivate();
				//active_objects.FindObject()->FindComponentByType<ActivObject>()->SetCurrentActive(true);
			}
		}
	}
	/*else
	{
		m_GameObject->FindComponentByType<Graphics::Renderable>()->pTexture = ResourceManager::textures["Palanca off.png"].get();
		for (auto & active_objects : m_activableCogPaths)
		{
			active_objects.FindObject()->FindComponentByType<ActivObject>()->ToggleActivate();
			//active_objects.FindObject()->FindComponentByType<ActivObject>()->SetCurrentActive(true);
		}
	}*/

	//m_GameObject->FindComponentByType<Graphics::Renderable>()->pTexture = ResourceManager::textures["Palanca off.png"].get();

}

void Activable::LateUpdate()
{
	auto iterator = m_activableObjects.begin();
	GameObject* object = nullptr;

	while (iterator != m_activableObjects.end())
	{
		object = *iterator;
		if (object->m_alive == false)
		{
			iterator = m_activableObjects.erase(iterator);
			continue;
		}
		else
			iterator++;
	}

}
