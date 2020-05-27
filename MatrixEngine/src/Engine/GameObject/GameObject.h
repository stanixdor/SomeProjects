#pragma once

#include <string>
#include <algorithm>
#include <typeinfo>
#include <vector>
#include "Component.h"
#include "../Core/DataTypes.h"
#include "ComponentManager.h"
#include <ObjBase.h>
#include <Guiddef.h>
#include "GameObjectManager.h"
#include "../Transform/Transform2D.h"
#include "Engine/Platform/EventSystem/event.h"
enum GameObjectMode { Default, Menu, HUD, Inactive };

class GameObject
{
public:


	
	std::string m_name;
	std::string m_tag;
	bool m_alive;
	GameObject* m_parent;
	Transform2D* m_transform;
	Transform2D* m_localTransform;
	GameObjectMode mode;
	//GUID m_guid;
	size_t m_guid;

	typedef std::vector<ComponentPtr> component_vector;
	typedef component_vector::iterator component_vector_itr;
	typedef component_vector::const_iterator component_vector_const_itr;

	typedef std::vector<GameObject*> game_object_vector;
	typedef game_object_vector::iterator game_object_vector_itr;
	typedef game_object_vector::const_iterator game_object_vector_const_itr;


	void from_json(const Json::Value & val);
	void to_json(Json::Value & val);
	GameObject* Clone();
	void save_archetype(const std::string & filename, const std::string & path);

	component_vector m_components;
	game_object_vector m_children;

	GameObject(bool addTransform = true);
	GameObject(const std::string& l_name, bool addTransform = true);
	~GameObject(void) = default;

	void Destroy();

	template<typename T>
	T * AddComponent()
	{
	    auto l_component = std::make_unique<T>();
	    l_component->m_GameObject = this;
	    l_component->Initialize();
	    auto* ptr = l_component.get(); 
		m_components.push_back(std::move(l_component));
	    return ptr;
	}

	template <typename T>
	T* FindComponentByType()
	{
		for (component_vector_itr itr = m_components.begin(); itr != m_components.end(); ++itr)
		{
			if (T* l_type = dynamic_cast<T*>((*itr).get()))
				return l_type;
		}
		return nullptr;
	}

	GameObject* AddChild(std::string l_name);

	GameObject* FindChildByName(std::string l_name);
	std::vector<GameObject*> FindAllChildrenByName(std::string l_name);
	Component* FindComponentByName(std::string l_name);


    Messaging::EventHandler& GetEventHandler();

	bool EditableByGizmos = true;

private:
    Messaging::EventHandler mEventHandler;

};

