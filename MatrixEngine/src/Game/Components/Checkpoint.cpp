// ----------------------------------------------------------------------------
//	Copyright (C)DigiPen Institute of Technology.
//	Reproduction or disclosure of this file or its contents without the prior 
//	written consent of DigiPen Institute of Technology is prohibited.
//	
//	File Name:	    Checkpoint.cpp 
//	Author:			Javier Tafur, javier.tafur
// ---------------------------------------------------------------------------

#include "Game\Components\Checkpoint.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Engine/Physics/CollisionSystem.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "Events\OnPlayerDetected.h"
#include "Platform\Time\FrameRateController.h"
#include "EngineDebug\ConsolePrinting.h"
#include "Engine/Graphics/GraphicsSystem.h"
#include "Engine/Platform/Resource Manager/ResourceManager.h"
#include "Levels\LevelTest.h"



REFLECTION(Checkpoint, is_check);

glm::vec2 Checkpoint::respawnPosition = glm::vec2(-778.3875732421875f, -1383.57177734375f);


void Checkpoint::collisionStarted(const OnCollisionStarted& mEvent)
{
	if (mEvent.mData.obj1->getCollisionGroup().mName == "player" || mEvent.mData.obj2->getCollisionGroup().mName == "player")
	{
		is_check = true;
		std::cout << is_check << std::endl;

		m_GameObject->FindComponentByType<Graphics::Renderable>()->pTexture = ResourceManager::textures["Poster guardado.png"].get();
		Checkpoint::respawnPosition = this->m_GameObject->m_transform->GetPos();
	}
	
}

void Checkpoint::collisionPersisted(const OnCollisionPersisted&)
{
	//if(mEvent.mData.obj1->getCollisionGroup().mName == "player" || mEvent.mData.obj2->getCollisionGroup().mName == "player")
	//std::cout << "dibooo" << std::endl;
}

void Checkpoint::collisionEnded(const OnCollisionEnded&)
{

}


Checkpoint::Checkpoint()
{
	m_name = "Checkpoint";
}

void Checkpoint::Initialize()
{
	//COLLISION EVENTS:

	////register object and function
	using namespace Messaging;

	// register to the event:

	EventHandler* temp = &GetEventHandler();
	temp->register_handler(this, &Checkpoint::collisionStarted);
	temp->register_handler(this, &Checkpoint::collisionPersisted);
	temp->register_handler(this, &Checkpoint::collisionEnded);

	// subscribe this to this type of event
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
}

void Checkpoint::Update()
{

}