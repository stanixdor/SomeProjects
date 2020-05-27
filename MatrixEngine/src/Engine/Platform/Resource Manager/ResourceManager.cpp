#include "ResourceManager.h"
#include "EngineDebug\ConsolePrinting.h"
#include "Utils\Serialization.h"
#include <iostream>
#include <sstream>

std::map<std::string, Json::Value> ResourceManager::archetypes;
std::map<std::string, Json::Value> ResourceManager::levels;
std::map<std::string, std::unique_ptr<Graphics::Texture> > ResourceManager::textures;

std::string ResourceManager::GetFileExtension(const std::string & filename)
{
	return filename.substr(filename.find_last_of(".") + 1);
}
std::string ResourceManager::IgnoreFolderInString(const std::string& filename)
{
	return filename.substr(filename.find_last_of("\\") + 1);
}

std::vector<std::string> ResourceManager::GetFilesInDirectory(const std::string& path)
{
	std::vector<std::string> filesInDirectory;
	std::stringstream bufferToCaptureFilename;
	for (auto & p : fs::directory_iterator(path))
	{
		bufferToCaptureFilename << p;
		filesInDirectory.push_back(IgnoreFolderInString(bufferToCaptureFilename.str()));
		bufferToCaptureFilename.str(std::string());
	}
	return filesInDirectory;
}

void ResourceManager::AddResource(std::string&)
{
	
	/*if (extension == "archetype")
	{

	}
	else if (extension == "bmp" || extension == "jpg" || extension == "jpeg" || extension == "png")
	{

	}
	else if (extension == "mp3" || extension == "wav")
	{

	}
	else if (extension == "collisiontable")
	{

	}
	else
	{
		std::cout << "Unknown file extension\n";
	}*/
}

void ResourceManager::GetLevels(const std::string & path)
{
	auto files = GetFilesInDirectory(path);
	std::string extension;

	for (const auto& file : files)
	{
		extension = GetFileExtension(file);
		if (extension != "level")
			Console::WriteLine(std::stringstream("Warning: Attemped to load a  " + extension + "  file as a level"));
		else
		{
			ResourceManager::levels[file] = serialization::FileToJson(file, path);
		}
	}

	for (auto it = ResourceManager::levels.begin(); it != ResourceManager::levels.end();)
	{
		if (std::find(files.begin(), files.end(), it->first) == files.end())
			it = ResourceManager::levels.erase(it);
		else ++it;

	}
}



void ResourceManager::GetArchetypes(const std::string & path)
{

	auto files = GetFilesInDirectory(path);
	std::string extension;

	for (const auto& file : files)
	{
		extension = GetFileExtension(file);
		if (extension != "archetype")
			Console::WriteLine(std::stringstream("Warning: Attemped to load a  " + extension + "  file as an archetype"));
		else
		{
			ResourceManager::archetypes[file] = serialization::FileToJson(file, path);
		}
	}

	for (auto it = ResourceManager::archetypes.begin(); it != ResourceManager::archetypes.end();)
	{
		if (std::find(files.begin(), files.end(), it->first) == files.end())
			it = ResourceManager::archetypes.erase(it);
		else ++it;
	}
}

void ResourceManager::GetTextures(const std::string & path)
{
	//2:14
	
	auto files = GetFilesInDirectory(path);
	std::string extension;

	for (const auto& file : files)
	{
		extension = GetFileExtension(file);

		//if its not a texture we write stuff
		if (extension != "bmp" && extension != "jpg" && extension != "jpeg" && extension != "png")
			Console::WriteLine(std::stringstream("Warning: Attemped to load a  " + extension + "  file as a texture"));
		//otherwise 
		else if(ResourceManager::textures.find(file) == ResourceManager::textures.end())
		{
			ResourceManager::textures[file] = std::make_unique<Graphics::Texture>(path + file);
		}
	}
	for (auto it = ResourceManager::textures.begin(); it != ResourceManager::textures.end();)
	{
		if (std::find(files.begin(), files.end(), it->first) == files.end())
			it = ResourceManager::textures.erase(it);
		else ++it;
	}


	//std::sort(ResourceManager::textures.begin(), ResourceManager::textures.end());
}


void ResourceManager::RefreshResources()
{
	GetLevels("Data/Levels/");
	GetArchetypes("Data/Archetypes/");
	GetTextures("Data/Textures/");
	Console::WriteLine(std::stringstream("Resources updated succesfully"));
}

