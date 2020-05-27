#include "PropulsionObject.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "GameObject\GameObject.h"
#include "Physics/CollisionData.h"
#include "Physics\Collider.h"
#include "Physics\CollisionTable.h"

PropulsionObject::PropulsionObject() : Component("Propulsion")
{

}

void PropulsionObject::Update()
{
}

void PropulsionObject::Initialize()
{
	Owner = this->GetOwner();	//Save the object

	using namespace Messaging;
	Messaging::EventHandler* temp = &Owner->GetEventHandler();

	Owner->GetEventHandler().register_handler(this, &PropulsionObject::collisionStarted);
	Owner->GetEventHandler().register_handler(this, &PropulsionObject::collisionPersisted);
	Owner->GetEventHandler().register_handler(this, &PropulsionObject::collisionEnded);

	////subscribe this handler to this type of event
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
}

void PropulsionObject::collisionStarted(const OnCollisionStarted& mEvent)
{
	if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("TongueCollisionGroup") || mEvent.mData.obj2->getCollisionGroup() == CollisionGroup::GetCollisionGroup("TongueCollisionGroup"))
	{
		//std::cout << "Collided with tongue" << std::endl;
	}
}

void PropulsionObject::collisionPersisted(const OnCollisionPersisted&)
{

}

void PropulsionObject::collisionEnded(const OnCollisionEnded&)
{

}