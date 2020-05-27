// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    ActivObject.cpp 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "Game/Components/ActivObject.h"
#include "Game/Components/ActivableComponent.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Events\OnPlayerDetected.h"
#include "Engine/Graphics/GraphicsSystem.h"
#include "Engine/Platform/Resource Manager/ResourceManager.h"
#include "Levels\LevelTest.h"
#include <iostream>
#include "GameObject\GameObject.h"
#include "Physics\Collider.h"


//! ---------------------------------------------------------------------------
// \fn	Collisions functions for the Laser
// ---------------------------------------------------------------------------


void ActivObject::collisionStarted(const OnCollisionStarted& mEvent)
{
	ActivObject_collisionStarted(mEvent);
}

void ActivObject::collisionPersisted(const OnCollisionPersisted& mEvent)
{
	ActivObject_collisionPersisted(mEvent);
}

void ActivObject::collisionEnded(const OnCollisionEnded& mEvent)
{
	ActivObject_collisionEnded(mEvent);
}


ActivObject::ActivObject() : is_active(true)
{

}

void ActivObject::Initialize()
{
	m_name = "ActivObject";
	is_active = original_activate;

	//this->GetOwner()->FindComponentByType<Collider>()->isGhost = true;

	//register object and function
	using namespace Messaging;

	//register to the event:

	EventHandler* temp = &GetEventHandler();
	temp->register_handler(this, &ActivObject::collisionStarted);
	temp->register_handler(this, &ActivObject::collisionPersisted);
	temp->register_handler(this, &ActivObject::collisionEnded);

	// subscribe this to this type of event
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());

	ActivObjectInitialize();

}

void ActivObject::Update()
{
	ActivObjectUpdate();
}


bool ActivObject::SetCurrentActive(bool activeState)
{
	return is_active = activeState;
}

bool ActivObject::ToggleActivate()
{
	is_active = !is_active;
	return is_active;
}