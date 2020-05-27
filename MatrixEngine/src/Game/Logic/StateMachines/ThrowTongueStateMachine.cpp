//#include "ThrowTongueStateMachine.h"
//#include "GameObject\GameObject.h"
//#include "Engine\Platform\EventSystem\event_dispatcher.h"

//static GameObject * Player;

//---------------------------------------------------------------------------/
// THROW TONGUE STATE																 /
//---------------------------------------------------------------------------/

//void ThrowTongue_State::collisionStarted(const OnCollisionStarted&)
//{
//}
//
//void ThrowTongue_State::collisionPersisted(const OnCollisionPersisted&)
//{}
//
//void ThrowTongue_State::collisionEnded(const OnCollisionEnded&)
//{}
//
//void ThrowTongue_State::Enter()
//{
//	std::cout << "Throw tongue Entered" << std::endl;
//}
//
//void ThrowTongue_State::initialize()
//{
//	Player = mOwnerStateMachine->GetOwner();
//
//	using namespace Messaging;
//	Messaging::EventHandler* temp = &mOwnerStateMachine->GetOwner()->GetEventHandler();
//
//	////register object and function
//	mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &ThrowTongue_State::collisionStarted);
//	mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &ThrowTongue_State::collisionPersisted);
//	mOwnerStateMachine->GetOwner()->GetEventHandler().register_handler(this, &ThrowTongue_State::collisionEnded);
//
//	////subscribe this handler to this type of event
//	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionStarted>());
//	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionPersisted>());
//	Messaging::EventDispatcher::get_instance().subscribe(*temp, type_of<OnCollisionEnded>());
//}
//
//void ThrowTongue_State::Exit()
//{}
//
//void ThrowTongue_State::Update()
//{
//}