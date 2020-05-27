// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    Door.cpp 
//	Purpose:		a component that is activable and have specific functions
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "Components\Door.h"
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
#include "Platform\MessageWindow\MessageWindow.h"
#include "Physics\Collider.h"
#include "Engine\Graphics\GraphicsComponents\Renderable.h"


void Door::ActivObject_collisionStarted(const OnCollisionStarted&)
{

}

void Door::ActivObject_collisionPersisted(const OnCollisionPersisted&)
{


}

void Door::ActivObject_collisionEnded(const OnCollisionEnded&)
{

}





void Door::ActivObjectInitialize()
{
	m_name = "Door";

	m_rend = m_GameObject->FindComponentByType<Graphics::Renderable>();
	m_collider = this->GetOwner()->FindComponentByType<Collider>();
}


void Door::ActivObjectUpdate()
{
	if (m_collider == nullptr)
	{
		m_collider = this->GetOwner()->FindComponentByType<Collider>();

		std::string text;
		std::cout << text + "Object " + m_GameObject->m_name + "doesnt have a collider required by " + m_name << std::endl;
		
		return;
	}
	if(!m_rend)
		m_rend = m_GameObject->FindComponentByType<Graphics::Renderable>();

	if (is_active == false)
	{
		m_rend->mColor.w = 0;
		m_collider->isGhost = true;

	}
	else
	{
		m_rend->pTexture = ResourceManager::textures["Puerta metalica.png"].get();
		m_rend->mColor.w = 255;
		m_collider->isGhost = false;
	}
}