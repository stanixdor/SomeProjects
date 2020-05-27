#include "Level.h"
#include "Platform\Resource Manager\ResourceManager.h"
#include "Physics\CollisionSystem.h"

std::map<std::string, std::unique_ptr<Level>> Levels;
Level* currentLevel = nullptr;

void ResetLevel(const std::string name, const std::string& folder)
{
	if (currentLevel != nullptr)
	{
		currentLevel->UnloadResources();
		currentLevel->ShutDown();
		CollisionSystem::Instance()->LastFrameCollisions.clear();
	}
	//currentLevel = Levels[name].get();
	currentLevel->Initialize();
	currentLevel->LoadResources(name, folder);
}