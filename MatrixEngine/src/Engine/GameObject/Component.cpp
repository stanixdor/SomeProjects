#include "Component.h"
#include "GameObject.h"
#include "Engine\Platform\EventSystem\event.h"

Component::Component(void)
{
	this->m_name = "Component";
	this->m_alive = true;
	this->m_GameObject = nullptr;
	this->Awake();
	//HRESULT l_result = CoCreateGuid(&m_guid);
	m_guid = (size_t)this;
}

Component::Component(std::string l_name)
{
	this->m_name = l_name;
	this->m_alive = true;
	this->m_GameObject = nullptr;
	this->Awake();
	//HRESULT l_result = CoCreateGuid(&m_guid);
	m_guid = (size_t)this;
}

Component::~Component(void)
{

}

GameObject* Component::GetOwner(void)
{
	return m_GameObject;
}

Messaging::EventHandler& Component::GetEventHandler()
{
    return GetOwner()->GetEventHandler();
}

void Component::Destroy()
{
	m_alive = false;
}
void Component::Awake()
{

}
void Component::Initialize()
{

}
void Component::FixedUpdate()
{

}
void Component::Update()
{

}
void Component::LateUpdate()
{

}
//void Component::Render()
//{
//
//}