// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    Steam.cpp 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "Game\Components\SteamComponent.h"
#include "Engine/Physics/CollisionEvents/OnCollisionEvent.h"
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
// \fn	Collisions functions for the Steam
// ---------------------------------------------------------------------------

void Steam::ActivObject_collisionStarted(const OnCollisionStarted& mEvent)
{
	if (mEvent.mData.obj1->getCollisionGroup().mName == "player" || mEvent.mData.obj2->getCollisionGroup().mName == "player")
	{
		if (m_collider->isGhost == false)
			objectManager.FindObjectByName("player")->m_transform->mPosition = Checkpoint::respawnPosition;
	}
}


void Steam::ActivObject_collisionPersisted(const OnCollisionPersisted&)
{
}

void Steam::ActivObject_collisionEnded(const OnCollisionEnded&)
{
}

// ctor :

void Steam::ActivObjectInitialize()
{
	m_name = "Steam";
	m_Anim = m_GameObject->FindComponentByType<Graphics::AnimComp>();
	m_collider = this->GetOwner()->FindComponentByType<Collider>();
}


void Steam::ActivObjectUpdate()
{
	if (m_collider == nullptr)
	{
		m_collider = this->GetOwner()->FindComponentByType<Collider>();

		std::string text;
		std::cout << text + "Object " + m_GameObject->m_name + "doesnt have a collider required by " + m_name << std::endl;

		return;
	}
	if (!m_Anim)
		m_Anim = m_GameObject->FindComponentByType<Graphics::AnimComp>();


	if (is_active == false)
	{
		m_Anim->SetAlpha(0.f);
		m_collider->isGhost = true;

	}
	else
	{
		m_Anim->SetAlpha(1.f);
		m_collider->isGhost = false;
	}
}