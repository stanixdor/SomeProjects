#include "Pipe.h"
#include "Engine\Platform\Input\Input.h"
#include "Engine\GameObject\GameObjectManager.h"
#include "Engine\Platform\EventSystem\event_dispatcher.h"
#include "FakeTongue.h"
#include "Tongue.h"
#include "Physics\Collider.h"
#include "Physics\RigidBody.h"
REFLECTION(Pipe, m_name, exitCogPath);

bool Pipe::fakeTongueHasFullLength = false;

Pipe::Pipe()
{
	m_name = "Pipe System";
}


void Pipe::Initialize()
{
	//register object and function
	using namespace Messaging;

	// register to the event:

	EventHandler* temp = &GetEventHandler();
	temp->register_handler(this, &Pipe::onCollisionStarted);


	// subscribe this to this type of event
	EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());

	//m_activableObjects.push_back(cp.target);
	exitPipe = exitCogPath.FindObject();

}

void Pipe::Update()
{

	if (exitPipe == nullptr)
	{
		exitPipe = exitCogPath.FindObject();
	}
}


void Pipe::onCollisionStarted(const OnCollisionStarted & mEvent)
{
	if (exitPipe != nullptr)
		if (mEvent.mData.eitherObjectHasACollisionGroupOf(CollisionGroup::GetCollisionGroup("TongueCollisionGroup")))
		{
			TongueComponent* tongue = nullptr;

			if (mEvent.mData.obj1->getCollisionGroup() == CollisionGroup::GetCollisionGroup("TongueCollisionGroup"))
				tongue = mEvent.mData.obj1->m_GameObject->FindComponentByType<TongueComponent>();
			else
				tongue = mEvent.mData.obj2->m_GameObject->FindComponentByType<TongueComponent>();

			GameObject* newTongue = objectManager.AddGameObject("FakeTongue");

			FakeTongue* fakedTongue = newTongue->AddComponent<FakeTongue>();

			if (!fakeTongueHasFullLength)
				fakedTongue->MaxLength = tongue->MaxLength - tongue->Length;

			RigidBody* rb = newTongue->AddComponent<RigidBody>();
			rb->isStatic = false;
			Collider* collider = newTongue->AddComponent<Collider>();
			collider->addCollisionGroup(CollisionGroup::GetCollisionGroup("faketonguecollisiongroup"));

			fakedTongue->placeTongue(*(exitPipe->m_transform));
			createdTongue = newTongue;
		}
}