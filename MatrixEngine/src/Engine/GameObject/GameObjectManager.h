#pragma once

#include "Component.h"
#include <vector> 
//#include "Components/Renderer.h" 
#include "GameObject.h" 

class GameObjectManager
{
public:

	typedef std::vector<GameObjectPtr> game_object_vector;
	typedef game_object_vector::iterator game_object_vector_itr;
	typedef game_object_vector::const_iterator game_object_vector_const_itr;

	game_object_vector m_gameObjects;
	//std::vector<std::unique_ptr<GameObject> > menuObjects;

	GameObjectManager(void);
	~GameObjectManager(void);

	void Destroy();
	void DestroyDeathObjects();

	GameObject* AddGameObject(GameObjectPtr&& l_gameObject);
	GameObject* AddGameObject(std::string l_name, bool addTransform = true);
	GameObject* AddGameObjectToSpace(std::string l_name, bool addTransform = true, const int gameObjectMode = 0);

	void FixedUpdate();
	void Update();
	void LateUpdate();

	//void Render();

	void SaveToFile(const std::string & filename, const std::string & path);
	void LoadFromFile(const std::string & filename, const std::string & path);

	GameObject* FindObjectByName(std::string l_name);
	std::vector<GameObject*> FindAllObjectsByName(std::string l_name);

};

extern GameObjectManager objectManager;