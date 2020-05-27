#include "GameObjectManager.h"
#include "GameObject.h"
#include "Component.h"
#include "Utils\CogPath.h"
#include "../Utils/Serialization.h"
#include <typeinfo>
#include "Graphics\GraphicsSystem.h"

GameObjectManager objectManager;


GameObjectManager::GameObjectManager(void)
{

}
GameObjectManager::~GameObjectManager(void)
{

}

void GameObjectManager::Destroy()
{
	for (auto& object : m_gameObjects)
	{
		object->Destroy();
	}
	//m_gameObjects.clear();
}
void GameObjectManager::DestroyDeathObjects()
{
	for (game_object_vector_itr itr = m_gameObjects.begin(); itr != m_gameObjects.end();)
	{
		if (!(*itr)->m_alive)
		{
			for (auto& child : (*itr)->m_children)
				child->m_parent = nullptr;
			itr = m_gameObjects.erase(itr);
		}
		else
		{
			for (auto component = (*itr)->m_components.begin(); component != (*itr)->m_components.end();)
			{
				if (!(*component)->m_alive)
				{
					component = (*itr)->m_components.erase(component);
				}
				else ++component;
			}
			++itr;
		}
	}

	//for (game_object_vector_itr itr = menuObjects.begin(); itr != menuObjects.end();)
	//{
	//	if (!(*itr)->m_alive)
	//	{
	//		for (auto& child : (*itr)->m_children)
	//			child->m_parent = nullptr;
	//		itr = menuObjects.erase(itr);
	//	}
	//	else
	//	{
	//		for (auto component = (*itr)->m_components.begin(); component != (*itr)->m_components.end();)
	//		{
	//			if (!(*component)->m_alive)
	//			{
	//				component = (*itr)->m_components.erase(component);
	//			}
	//			else ++component;
	//		}
	//		++itr;
	//	}
	//}
}
GameObject* GameObjectManager::AddGameObject(GameObjectPtr&& l_gameObject)
{
	this->m_gameObjects.push_back(std::move(l_gameObject));
	int l_position = static_cast<int>(m_gameObjects.size() - 1);
	return m_gameObjects[l_position].get();
}
GameObject* GameObjectManager::AddGameObject(std::string l_name, bool addTransform)
{

	this->m_gameObjects.push_back(std::make_unique<GameObject>(l_name, addTransform));
	return m_gameObjects.back().get();
}

GameObject * GameObjectManager::AddGameObjectToSpace(std::string l_name, bool addTransform, const int gameObjectMode)
{
	ENGINE_UNUSED(addTransform);
	ENGINE_UNUSED(gameObjectMode);
	GameObject* helper;
	
	GameObjectMode mode = static_cast<GameObjectMode>(gameObjectMode);
	switch (mode)
	{
		case GameObjectMode::Default:
			return AddGameObject(l_name, addTransform);
		
		case GameObjectMode::Menu:
			m_gameObjects.push_back(std::make_unique<GameObject>(l_name, addTransform));
			helper = m_gameObjects.back().get();
			helper->mode = static_cast<GameObjectMode>(gameObjectMode);
			return helper;
	}
	
	return m_gameObjects.back().get();
}

void GameObjectManager::FixedUpdate()
{
	for (auto& object : m_gameObjects)
		for (auto& component : object->m_components)
			component->FixedUpdate();
}
void GameObjectManager::Update()
{

	for (size_t counter = 0; counter < m_gameObjects.size(); counter++)
	{
		GameObject* temp = m_gameObjects[counter].get();

		if (temp->mode == GameObjectMode::Inactive)
			continue;
		if (!PAUSE)
		{
			for (size_t componentCounter = 0; componentCounter < temp->m_components.size(); componentCounter++)
			{
				if (!playMode)
				{
					if (!temp->m_components[componentCounter]->updateInEditMode)
						continue;
				}
				//else if (PAUSE)
				//{
				//	if (!temp->m_components[componentCounter]->updateInEditPauseMenu)
				//		continue;
				//}
				temp->m_components[componentCounter]->Update();
			}
		}
		else if (temp->mode == GameObjectMode::Menu)
		{
			for (size_t componentCounter = 0; componentCounter < temp->m_components.size(); componentCounter++)
				temp->m_components[componentCounter]->Update();
		}
	}

        //for (auto it2 = temp->m_components.begin(); it2 != temp->m_components.end(); it2++)
        //{
        //    (*it2)->Update();
        //}
	//for (auto& object : m_gameObjects)
	//	for (auto& component : object->m_components)
	//		component->Update();
}
void GameObjectManager::LateUpdate()
{
	for (auto& object : m_gameObjects)
	{
		for (auto& component : object->m_components)
		{
			component->LateUpdate();
		}
		//remove game objects that no longer exists from parent vector
		//auto childVectorIterator = object->m_children.begin();
		//while (childVectorIterator != object->m_children.end())
		//{
		//	if ((*childVectorIterator)->m_alive == false)
		//	{
		//		childVectorIterator = object->m_children.erase(childVectorIterator);
		//	}
		//	else
		//		childVectorIterator++;
		//
		//}
	}
}

//void GameObjectManager::Render()
//{
//	ComponentManager::getInstance()->Render();
//}

GameObject* GameObjectManager::FindObjectByName(std::string l_name)
{
	GameObject* l_object = nullptr;
	for (size_t counter = 0; counter < m_gameObjects.size(); counter++)
	{
		GameObject* gameObject = m_gameObjects[counter].get();
		if (gameObject->m_name == l_name)
		{
			l_object = gameObject;
			break;
		}
	}
	return l_object;
}
std::vector<GameObject*> GameObjectManager::FindAllObjectsByName(std::string l_name)
{
	std::vector<GameObject*> l_objects;

	for (const auto& gameObject : m_gameObjects)
	{
		if (gameObject->m_name == l_name)
			l_objects.push_back(gameObject.get());
	}
	return l_objects;

}

void GameObjectManager::SaveToFile(const std::string & filename, const std::string & path)
{
	/*SaveFileDialog* saveFileDialog1 = gcnew SaveFileDialog;
	saveFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
	saveFileDialog1->FilterIndex = 2;
	saveFileDialog1->RestoreDirectory = true;
	if (saveFileDialog1->ShowDialog() == ::DialogResult::OK)
	{
		if ((myStream = saveFileDialog1->OpenFile()) != nullptr)
		{

			// Code to write the stream goes here.
			myStream->Close();
		}
	}*/
	Json::Value Level;
	Json::Value& objects = Level["objects"];

	for (const auto& object : m_gameObjects)
	{
		Json::Value objectJson;
		object->to_json(objectJson);
		objects.append(objectJson);
	}
	serialization::JsonToFile(Level, filename, path);
}

void GameObjectManager::LoadFromFile(const std::string & filename, const std::string & path)
{
	Json::Value Level = serialization::FileToJson(filename, path);

	const Json::Value& objects = Level["objects"];

	for (const Json::Value & objectJson : objects)
	{
		GameObject * const object = AddGameObjectToSpace(objectJson["Name"].asString(), false, (objectJson["Mode"].asInt()));

		object->from_json(objectJson);
	}

}