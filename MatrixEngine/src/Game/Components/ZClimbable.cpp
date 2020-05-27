#include "ZClimbable.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "GameObject\GameObject.h"
#include "Physics/CollisionData.h"
#include "Physics\RigidBody.h"
#include "Physics\CollisionTable.h"
#include "Engine\Logic\StateMachine\StateMachine.h"
#include "Game\Logic\StateMachines\PlayerStateMachine.h"
#include "Levels\LevelTest.h"

ZClimbableObject::ZClimbableObject() : Component("ZClimbableObject")
{
	m_name = "ZClimbableObject";
}

void ZClimbableObject::Update()
{
	if (Playerptr == nullptr)
	{
		Playerptr = objectManager.FindObjectByName("player");
		//std::cout << "PlayerNULL" << std::endl;
	}

	if (Owner->FindComponentByType<Collider>() != nullptr)
	{
		if (Owner->FindComponentByType<Collider>()->isGhost == false)
		{
			Owner->FindComponentByType<Collider>()->isGhost = true;
		}
	}
}

void ZClimbableObject::Initialize()
{
	Owner = this->GetOwner();	//Save the object

	//Owner->AddComponent<Collider>();

	
	Playerptr = objectManager.FindObjectByName("player");

	using namespace Messaging;
	Messaging::EventHandler* temp = &Owner->GetEventHandler();

	Owner->GetEventHandler().register_handler(this, &ZClimbableObject::collisionStarted);
	Owner->GetEventHandler().register_handler(this, &ZClimbableObject::collisionPersisted);
	Owner->GetEventHandler().register_handler(this, &ZClimbableObject::collisionEnded);

	////subscribe this handler to this type of event
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
}

void ZClimbableObject::collisionStarted(const OnCollisionStarted& mEvent)
{
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Player") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Player"))
	{
		StateMachine * PlayerSM = Playerptr->FindComponentByType<StateMachine>();

		if (PlayerSM != nullptr)
		{
			PlayerSM->ChangeState<PlayerSM::ZClimb_State>();

			PlayerSM->GetState<PlayerSM::ZClimb_State>()->WallClimbing = Owner;
		}

	}
}

void ZClimbableObject::collisionPersisted(const OnCollisionPersisted&)
{
}

void ZClimbableObject::collisionEnded(const OnCollisionEnded& mEvent)
{
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Player") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Player"))
	{
		StateMachine * PlayerSM = Playerptr->FindComponentByType<StateMachine>();

		PlayerSM->ChangeState<PlayerSM::Idle_State>();

	}
}