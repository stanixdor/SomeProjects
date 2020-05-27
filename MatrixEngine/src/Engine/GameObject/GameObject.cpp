#include "GameObject.h"
#include "..\Base\IBase.h"

GameObject::GameObject(bool addTransform) : mEventHandler(Messaging::EventHandler(*this))
{
	this->m_name = "GameObject";
	this->m_tag = "";
	this->m_parent = nullptr;
	this->m_alive = true;
	mode = GameObjectMode::Default;
	if (addTransform)
	{
		this->m_transform = this->AddComponent<Transform2D>();
		//this->m_localTransform = this->AddComponent<Transform2D>();
		//m_localTransform->mIsChild = true;
		//m_localTransform->m_name = "Child Transform";
	}
	//HRESULT l_result = CoCreateGuid(&m_guid);
	m_guid = (size_t)this;
}
GameObject::GameObject(const std::string& l_name, bool addTransform) : mEventHandler(Messaging::EventHandler(*this))
{
	this->m_name = l_name;
	this->m_tag = "";
	this->m_parent = nullptr;
	this->m_alive = true;
	mode = GameObjectMode::Default;
	if (addTransform)
	{
		this->m_transform = this->AddComponent<Transform2D>();
		//this->m_localTransform = this->AddComponent<Transform2D>();
		//m_localTransform->mIsChild = true;
		//m_localTransform->m_name = "Child Transform";

	}
	//this->m_transform = 
	//HRESULT l_result = CoCreateGuid(&m_guid);
	m_guid = (size_t)this;
}


void GameObject::Destroy()
{	
	m_alive = false;

	for (auto& comp : m_components)
		comp->m_alive = false;
	for (auto& child : m_children)
		child->Destroy();
}

void GameObject::from_json(const Json::Value & val)
{
	m_guid = static_cast<size_t>(val["ID"].asUInt64());
	const Json::Value & components = val["components"];

	for (const Json::Value & componentJson : components)
	{
		const auto component_type = componentJson["Type"].asString();

		const auto it = Creators.find(component_type);

		if (it == Creators.end())
		{
			MessageBoxA(nullptr,
				("The creator for the type with the name \""
					+ component_type
					+ "\" couldn't be found. Make sure to register a component to be able to read it from file.\n").c_str(),
				"CREATOR FOR COMPONENT NOT FOUND",
				MB_ICONERROR | MB_OK);

			//std::abort();
			continue;
		}

		m_components.push_back(std::unique_ptr<Component>(static_cast<Component *>(it->second->Create().release())));
		m_components.back()->m_GameObject = this;
		m_components.back()->from_json(componentJson["data"]);
		m_components.back()->Initialize();
		m_components.back()->m_guid = static_cast<size_t>(componentJson["ID"].asUInt64());
		mode = static_cast<GameObjectMode>(val["Mode"].asInt());
	}
}

void GameObject::to_json(Json::Value & val)
{
	val["Name"] = m_name;
	val["ID"] = m_guid;
	val["Mode"] = static_cast<int>(mode);
	Json::Value& components = val["components"];
	for (const auto& component : m_components)
	{
		Json::Value componentJson;
		componentJson["Type"] = typeid(*component.get()).name();
		componentJson["ID"] = component->m_guid;
		Json::Value & component_data = componentJson["data"];
		component->to_json(component_data);
		components.append(componentJson);
	}
}

GameObject* GameObject::Clone()
{
	Json::Value val;
	to_json(val);
	GameObject* clonedObject = objectManager.AddGameObject(m_name, false);
	clonedObject->from_json(val);

	clonedObject->m_guid = (size_t)(clonedObject);
	return clonedObject;
}

void GameObject::save_archetype(const std::string & filename, const std::string & path)
{
	Json::Value archetype;
	to_json(archetype);
	serialization::JsonToFile(archetype, filename, path);
}

GameObject* GameObject::AddChild(std::string l_name)
{
	auto l_gameObject = std::make_unique<GameObject>(l_name);
	l_gameObject->m_parent = this;
	auto* ptr = l_gameObject.get();
	objectManager.AddGameObject(std::move(l_gameObject));

	//l_gameObject->Create();
	this->m_children.push_back(ptr);
	return ptr;
}

GameObject* GameObject::FindChildByName(std::string l_name)
{
	GameObject* l_object = nullptr;
	for (auto& child : m_children)
	{
		if (child->m_name == l_name)
		{
			l_object = child;
			break;
		}
	}
	return l_object;
}
std::vector<GameObject*> GameObject::FindAllChildrenByName(std::string l_name)
{
	std::vector<GameObject*> l_objects;

	for (auto& child : m_children)
	{
		if (child->m_name == l_name)
			l_objects.push_back(child);
	}
	return l_objects;
}

Component* GameObject::FindComponentByName(std::string l_name)
{
	for (component_vector_itr itr = m_components.begin(); itr != m_components.end(); ++itr)
	{
		if ((*itr)->m_name == l_name)
			return (*itr).get();
	}
	return nullptr;
}

Messaging::EventHandler& GameObject::GetEventHandler()
{
    return mEventHandler;
}