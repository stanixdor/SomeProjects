#include "FlyComponent.h"
#include "Engine\Physics\CollisionEvents\OnCollisionEvent.h"
#include "Physics\CollisionTable.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "GameObject\GameObject.h"
#include "Platform\Input\Input.h"
#include "Physics\RigidBody.h"
#include "Engine/Graphics/GraphicsSystem.h"
#include "Platform\Resource Manager\ResourceManager.h"
#include "Physics\Collider.h"
#include "Pipe.h"
#include "Physics\RigidBody.h"
#include "Components/CamouflageComponent.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"

REFLECTION(FlyComponent, InitialPosition);

FlyComponent::FlyComponent() : Component("FlyComponent")
{
	m_name = "FlyComponent";
}

void FlyComponent::Update()
{
	if (!Stop)
		Idle();
}

void FlyComponent::Initialize()
{
	Owner = this->GetOwner();	//Save the owner of the component
	Owner->m_transform->mPosition = InitialPosition;

	using namespace Messaging;
	Messaging::EventHandler* temp = &(GetOwner()->GetEventHandler());


	temp->register_handler(this, &FlyComponent::onCollisionStarted);
	//Owner->GetEventHandler().register_handler(this, &FakeTongue::collisionPersisted);
	//Owner->GetEventHandler().register_handler(this, &FakeTongue::collisionEnded);
	//
	//subscribe this handler to this type of event
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
}

void FlyComponent::onCollisionStarted(const OnCollisionStarted& mEvent)
{
	if (teleported)
		return;

	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("CamouflageFlyCollisionGroup") && mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("PipeCollisionGroup"))
	{
		Pipe* pipe = mEvent.mData.obj2->m_GameObject->FindComponentByType<Pipe>();
		if (pipe)
		{
			m_GameObject->m_transform->mPosition = pipe->exitPipe->m_transform->mPosition;
			teleported = true;
		}
	}
	if (mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("CamouflageFlyCollisionGroup") && mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("PipeCollisionGroup"))
	{
		Pipe* pipe = mEvent.mData.obj1->m_GameObject->FindComponentByType<Pipe>();
		if (pipe)
		{
			m_GameObject->m_transform->mPosition = pipe->exitPipe->m_transform->mPosition;
			teleported = true;
		}
	}
}

void FlyComponent::Idle()
{
	MoveUpDown();
	MoveRightLeft();
}

void FlyComponent::MoveUpDown()
{
	if (GoUp)
	{
		if (MovementCounterUpDown > MaxMovementY)
		{
			GoUp = false;
			MovementCounterUpDown = 0.0f;
		}

		Owner->m_transform->mPosition.y += glm::cos(MovementSpeedUpDown * MovementSpeedUpDown);

		MovementCounterUpDown += IncrementSpeed;
	}
	else
	{
		if (MovementCounterUpDown > MaxMovementY)
		{
			GoUp = true;
			MovementCounterUpDown = 0.0f;
		}

		Owner->m_transform->mPosition.y -= glm::cos(MovementSpeedUpDown * MovementSpeedUpDown);

		MovementCounterUpDown += IncrementSpeed;
	}
}
void FlyComponent::MoveRightLeft()
{
	if (GoRight)
	{
		if (MovementCounterRightLeft > MaxMovementX)
		{
			GoRight = false;
			MovementCounterRightLeft = 0.0f;
		}

		Owner->m_transform->mPosition.x += glm::cos(MovementCounterRightLeft / 2.5f);

		MovementCounterRightLeft += IncrementSpeed;
	}
	else
	{
		if (MovementCounterRightLeft > MaxMovementX)
		{
			GoRight = true;
			MovementCounterRightLeft = 0.0f;
		}

		Owner->m_transform->mPosition.x -= glm::cos(MovementCounterRightLeft / 2.5f);

		MovementCounterRightLeft += IncrementSpeed;
	}
}