#pragma once
#include <string>
#include <vector>
#include <map>
#include "Json\json.h"
#include "Utils\ISingleton.h"
#include "Graphics\Texture.h"
#include <filesystem>
#include <memory>

//#include <set>
namespace fs = std::experimental::filesystem;

class ResourceManager: public ISingleton<ResourceManager>
{
public:
	static std::string GetFileExtension(const std::string& filename);
	static void AddResource(std::string& filename);
	static void GetLevels(const std::string& path);
	static void GetArchetypes(const std::string& path);
	static void GetTextures(const std::string& path);
	static void RefreshResources();
	static std::map<std::string, Json::Value> archetypes;
	static std::map<std::string, Json::Value> levels;
	static std::map<std::string, std::unique_ptr<Graphics::Texture> > textures;

	static std::map<std::string, Graphics::Texture*> HackedTextures;
	static std::vector<std::string> GetFilesInDirectory(const std::string& path);
	static std::string IgnoreFolderInString(const std::string& filename);

private:


};

