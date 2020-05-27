#pragma once

#include <map>
#include <memory>

class Level
{
public:
	virtual void Initialize() = 0;
	virtual void LoadResources(const std::string& name, const std::string& folder) = 0;
	virtual void Update() = 0;
	virtual void UnloadResources() = 0;
	virtual void ShutDown() = 0;
};

extern std::map<std::string, std::unique_ptr<Level>> Levels;
extern Level* currentLevel;

void ResetLevel(const std::string name, const std::string& folder = "Data/Levels/");