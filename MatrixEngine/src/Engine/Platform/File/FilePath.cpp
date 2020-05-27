#include "FilePath.h"
#include <Windows.h>
#include <Shlwapi.h> // path file exits	

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	// FilePath
	FilePath::FilePath()
	{}
	FilePath::FilePath(const char * path)
	{
		// Parse
		ParsePath(path);
	}
	void FilePath::ParsePath(const char * path)
	{
		if (path != nullptr)
			mFullPath = path;

		// safeguard
		if (mFullPath.length() == 0)
			return;

		// replace the '/' character with '\' for unity
		std::size_t slash_pos = mFullPath.find_first_of("/");
		while (slash_pos != std::string::npos)
		{
			mFullPath.replace(slash_pos, 1, "\\");
			slash_pos = mFullPath.find_first_of("/");
		} 

		// parse the mFullPath and extract

		// mDirectory
		std::size_t dir_offset = mFullPath.find_last_of("\\");
		mDirectory = mFullPath.substr(0, dir_offset + 1);

		// mExtension
		std::size_t ext_offset = mFullPath.find_last_of(".");
		if (ext_offset != std::string::npos){
			mExtension = mFullPath.substr(ext_offset);
			// mFilename
			mFilename = mFullPath.substr(dir_offset + 1, ext_offset - dir_offset - 1);
		}
		else
		{
			mFilename = mFullPath.substr(dir_offset + 1);
		}
	}