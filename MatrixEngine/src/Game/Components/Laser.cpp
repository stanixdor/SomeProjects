// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    Laser.cpp 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "Game/Components/Laser.h"
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
#include "Checkpoint.h"

//! ---------------------------------------------------------------------------
// \fn	Collisions functions for the Laser
// ---------------------------------------------------------------------------


void Laser::ActivObject_collisionStarted(const OnCollisionStarted& mEvent)
{
	if (mEvent.mData.obj1->getCollisionGroup().mName == "player" || mEvent.mData.obj2->getCollisionGroup().mName == "player")
	{
		if(m_collider->isGhost == false)
			objectManager.FindObjectByName("player")->m_transform->mPosition = Checkpoint::respawnPosition;
	}
}

void Laser::ActivObject_collisionPersisted(const OnCollisionPersisted&)
{
	/*if (mEvent.mData.obj1->getCollisionGroup().mName == "player" || mEvent.mData.obj2->getCollisionGroup().mName == "player")
	{
		objectManager.FindObjectByName("player")->m_transform->mPosition = Checkpoint::respawnPosition;
	}*/
	
}

void Laser::ActivObject_collisionEnded(const OnCollisionEnded&)
{

}


void Laser::ActivObjectInitialize()
{
	m_name = "Laser";
	m_rend = m_GameObject->FindComponentByType<Graphics::Renderable>();
	m_collider = this->GetOwner()->FindComponentByType<Collider>();
}


void Laser::ActivObjectUpdate()
{
	if (is_active == false)
	{
		m_rend->SetAlpha(0.f);
		//m_collider->addCollisionGroup()
		m_collider->isGhost = true;

	}
	else
	{
		m_rend->SetAlpha(1.f);
		m_collider->isGhost = false;
	}
}