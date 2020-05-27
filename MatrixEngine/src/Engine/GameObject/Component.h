#pragma once
#include <string>
#include "../Utils/Serialization.h"
#include <ObjBase.h>
#include <Guiddef.h>
#include "../Base/IBase.h"


class GameObject;
namespace Messaging
{
    class EventHandler;
}

class Component : public serialization::Serializable, public IBase
{
	friend class GameObject;
public:
	std::string m_name;

	//GUID m_guid;
	size_t m_guid;

	GameObject* m_GameObject;

	bool m_alive;

	Component(void);
	Component(std::string l_name);
	virtual ~Component(void);

	// Gets the owner, only the gameobject class can modify this
	GameObject* GetOwner(void);
    Messaging::EventHandler& Component::GetEventHandler();

	bool updateInEditMode = false;
	bool updateInEditPauseMenu = false;

	virtual void Destroy();
	virtual void Awake();
	virtual void Initialize();
	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();
};
