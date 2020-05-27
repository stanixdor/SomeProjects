// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    CollectibleComponent.cpp 
//	Purpose:		use for the collect object fly for the player like coins
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "CollectibleComponent.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Engine\Physics\CollisionSystem.h"
#include "GameObject\GameObject.h"


unsigned CollectibleComponent::currentTotalCollectibles = 0;
unsigned CollectibleComponent::originalCollectibleNumber = 0;

CollectibleComponent::CollectibleComponent() : Component("CollectibleComponent")
{
	m_name = "CollectibleComponent";
	CollectibleComponent::currentTotalCollectibles++;
}


CollectibleComponent::~CollectibleComponent()
{
	// the coin has been deleted, the object has taken a coin
	CollectibleComponent::currentTotalCollectibles--;
	//std::cout << getCollectiblesTaken() << " = flies taken " << std::endl;
}

void CollectibleComponent::Update()
{

}

void CollectibleComponent::Initialize()
{
	Messaging::EventHandler* temp = &GetEventHandler();
	temp->register_handler(this, &CollectibleComponent::collisionStarted);


	// subscribe this to this type of event
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());

	originalCollectibleNumber = currentTotalCollectibles;
}


// restore the total number of coin for the level
unsigned CollectibleComponent::getCollectiblesTaken() const
{
	return originalCollectibleNumber - currentTotalCollectibles;
}


void  CollectibleComponent::collisionStarted(const OnCollisionStarted& mEvent)
{
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("TongueCollisionGroup") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("TongueCollisionGroup"))
	{

		//std::cout << originalCollectibleNumber << " = total flies" << std::endl;
		m_GameObject->Destroy(); // destroy the coin (has been taken)
	}
}