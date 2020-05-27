#include "ClimbableObject.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "GameObject\GameObject.h"
#include "Physics/CollisionData.h"
#include "Physics\RigidBody.h"
#include "Physics\CollisionTable.h"
#include "Engine\Logic\StateMachine\StateMachine.h"
#include "Game\Logic\StateMachines\PlayerStateMachine.h"
#include "Levels\LevelTest.h"
#include "Game/Logic/StateMachines/playerStateMachine.h"

ClimbableObject::ClimbableObject() : Component("Climbable")
{
	m_name = "Climbable";
}

void ClimbableObject::Update()
{
	if (Playerptr == nullptr)
	{
		Playerptr = objectManager.FindObjectByName("player");
		//std::cout << "PlayerNULL" << std::endl;
	}
}

void ClimbableObject::Initialize()
{
	Owner = this->GetOwner();	//Save the object
	Playerptr = objectManager.FindObjectByName("player");

	using namespace Messaging;
	Messaging::EventHandler* temp = &Owner->GetEventHandler();

	Owner->GetEventHandler().register_handler(this, &ClimbableObject::collisionStarted);
	Owner->GetEventHandler().register_handler(this, &ClimbableObject::collisionPersisted);
	Owner->GetEventHandler().register_handler(this, &ClimbableObject::collisionEnded);

	////subscribe this handler to this type of event
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
}

void ClimbableObject::collisionStarted(const OnCollisionStarted& mEvent)
{
	if (Playerptr == nullptr)
	{
		Playerptr = objectManager.FindObjectByName("player");
		if (Playerptr == nullptr)
		{
			return;
		}
	}
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Player"))
	{
		//std::cout << "Collided with player" << std::endl;

		//TODO: Check if player is above the climable object



		StateMachine * PlayerSM = Playerptr->FindComponentByType<StateMachine>();


		if (Owner->m_transform->GetScale().x > Owner->m_transform->GetScale().y)
		{
			if (this->GetOwner()->m_transform->mPosition.x + (this->GetOwner()->m_transform->mScale.x / 2.0f) < mEvent.mData.obj1->getTransform().mPosition.x + (mEvent.mData.obj1->getTransform().mScale.x / 2.0f)
				|| this->GetOwner()->m_transform->mPosition.x - (this->GetOwner()->m_transform->mScale.x / 2.0f) > mEvent.mData.obj1->getTransform().mPosition.x - (mEvent.mData.obj1->getTransform().mScale.x / 2.0f))
			{
				PlayerSM->ChangeState<PlayerSM::Idle_State>();
				return;
			}

			PlayerSM->ChangeState<PlayerSM::HorizontalClimb_State>();
			PlayerSM->GetState<PlayerSM::HorizontalClimb_State>()->WallClimbing = Owner;
		}
		else
		{
			if (this->GetOwner()->m_transform->mPosition.y + (this->GetOwner()->m_transform->mScale.y / 2.0f) < mEvent.mData.obj1->getTransform().mPosition.y + (mEvent.mData.obj1->getTransform().mScale.y / 2.0f))
			{
				if (PlayerSM->GetState<PlayerSM::Idle_State>()->getPlayerStateMachine()->OnAir == false)
				{
					PlayerSM->ChangeState<PlayerSM::Idle_State>();
				}
				else
				{
					PlayerSM->ChangeState<PlayerSM::Falling_Jump_State>();
				}

				//PlayerSM->ChangeState<PlayerSM::Idle_State>();
				return;
			}

			PlayerSM->ChangeState<PlayerSM::VerticalClimb_State>();
			PlayerSM->GetState<PlayerSM::VerticalClimb_State>()->WallClimbing = Owner;
		}

		//PlayerSM->ChangeState<PlayerSM::VerticalClimb_State>();
		//PlayerSM->GetState<PlayerSM::VerticalClimb_State>()->WallClimbing = Owner;
	}
	else if (mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("Player"))
	{
		//std::cout << "Collided with player" << std::endl;

		//TODO: Check if player is above the climable object



		StateMachine * PlayerSM = Playerptr->FindComponentByType<StateMachine>();


		if (Owner->m_transform->GetScale().x > Owner->m_transform->GetScale().y)
		{
			if (this->GetOwner()->m_transform->mPosition.x + (this->GetOwner()->m_transform->mScale.x / 2.0f) < mEvent.mData.obj1->getTransform().mPosition.x + (mEvent.mData.obj1->getTransform().mScale.x / 2.0f)
				|| this->GetOwner()->m_transform->mPosition.x - (this->GetOwner()->m_transform->mScale.x / 2.0f) > mEvent.mData.obj1->getTransform().mPosition.x - (mEvent.mData.obj1->getTransform().mScale.x / 2.0f))
			{
				PlayerSM->ChangeState<PlayerSM::Idle_State>();
				return;
			}

			PlayerSM->ChangeState<PlayerSM::HorizontalClimb_State>();
			PlayerSM->GetState<PlayerSM::HorizontalClimb_State>()->WallClimbing = Owner;
		}
		else
		{
			if (this->GetOwner()->m_transform->mPosition.y + (this->GetOwner()->m_transform->mScale.y / 2.0f) < mEvent.mData.obj2->getTransform().mPosition.y + (mEvent.mData.obj2->getTransform().mScale.y / 2.0f))
			{
				if (PlayerSM->GetState<PlayerSM::Idle_State>()->getPlayerStateMachine()->OnAir == false)
				{
					PlayerSM->ChangeState<PlayerSM::Idle_State>();
				}
				else
				{
					PlayerSM->ChangeState<PlayerSM::Falling_Jump_State>();
				}
				return;
			}

			PlayerSM->ChangeState<PlayerSM::VerticalClimb_State>();
			PlayerSM->GetState<PlayerSM::VerticalClimb_State>()->WallClimbing = Owner;
		}

		//PlayerSM->ChangeState<PlayerSM::VerticalClimb_State>();
		//PlayerSM->GetState<PlayerSM::VerticalClimb_State>()->WallClimbing = Owner;
	}
}

void ClimbableObject::collisionPersisted(const OnCollisionPersisted&)
{
}

void ClimbableObject::collisionEnded(const OnCollisionEnded&)
{
}