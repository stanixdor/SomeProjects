// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    Laser.cpp 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "Game/Components/Laser2.h"
#include "Game/Components/ActivableComponent.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Events\OnPlayerDetected.h"
#include "Engine/Graphics/GraphicsSystem.h"
#include "Engine/Platform/Resource Manager/ResourceManager.h"
#include "Levels\LevelTest.h"
#include "Checkpoint.h"
#include <iostream>
#include "GameObject\GameObject.h"
#include "Physics\Collider.h"


//! ---------------------------------------------------------------------------
// \fn	Collisions functions for the Laser
// ---------------------------------------------------------------------------


void Laser2::collisionStarted(const OnCollisionStarted& mEvent)
{
	if ((mEvent.mData.obj1->getCollisionGroup().mName == "player" || mEvent.mData.obj2->getCollisionGroup().mName == "player") && is_active)
	{
		objectManager.FindObjectByName("player")->m_transform->mPosition = Checkpoint::respawnPosition;
	}
}

void Laser2::collisionPersisted(const OnCollisionPersisted&)
{
	/*if (mEvent.mData.obj1->getCollisionGroup().mName == "player" || mEvent.mData.obj2->getCollisionGroup().mName == "player")
	{
		objectManager.FindObjectByName("player")->m_transform->mPosition = Checkpoint::respawnPosition;
	}*/
	
}

void Laser2::collisionEnded(const OnCollisionEnded&)
{

}


Laser2::Laser2() : is_active(true) 
{

}

void Laser2::Initialize()
{
	m_name = "Laser2";
	//Activable::lasers.push_back(this);

	this->GetOwner()->FindComponentByType<Collider>()->isGhost = true;

	//register object and function
	using namespace Messaging;

	 //register to the event:

	EventHandler* temp = &GetEventHandler();
	temp->register_handler(this, &Laser2::collisionStarted);
	temp->register_handler(this, &Laser2::collisionPersisted);
	temp->register_handler(this, &Laser2::collisionEnded);

	// subscribe this to this type of event
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
}

void Laser2::Update()
{
	auto Owner = GetOwner();

	Owner->FindComponentByType<Collider>()->isGhost = true;

	//is_active = false;

	if (tex == nullptr)
		tex = GetOwner()->FindComponentByType<Renderable>();
	if (timer.GetTimeSinceStart() <= 0.f)
	{
		timer.Start();
	}
	else if (timer.GetTimeSinceStart() >= 4.f && timer.GetTimeSinceStart() < 10.f)
	{
		is_active = false;
	}
	else if (timer.GetTimeSinceStart() >= 8.f)
	{
		timer.Reset();
		is_active = true;
	}
	if (tex != nullptr)
	{
		if (is_active)
		{
			tex->SetAlpha(1.f);
		}
		else
		{
			tex->SetAlpha(0.f);
		}
	}
	//std::cout << is_active << std::endl;
	//std::cout << timer.GetTimeSinceStart() << std::endl;
	
	/*if (is_active == false)
	{
		m_GameObject->FindComponentByType<Graphics::Renderable>()->pTexture = ResourceManager::textures["Bidon.png"].get();
	}
	else
	{
		m_GameObject->FindComponentByType<Graphics::Renderable>()->pTexture = ResourceManager::textures["Plataforma pinchos.png"].get();
	}*/
}