#pragma once

#include "../../Engine/Utils/EventSystem/event.h"
#include "../../Engine/GameObject/GameObject.h"

//struct dummyObject: public GameObject
//{
//	void dummyFunction(const TestEvent&)
//	{
//		std::cout << "dummyFunction was called" << std::endl;
//	}
//};

//class TestHandler : public MemberFunctionHandler<testObject,TestEvent>
//{
//public:
//	void call(const Event& event)
//	{
//		std::cout << "the event got called and the listener was notified" << std::endl;
//	}
// };