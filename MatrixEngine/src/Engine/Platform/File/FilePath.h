// ----------------------------------------------------------------------------
#ifndef FILE_PATH_H_
#define FILE_PATH_H_
// ----------------------------------------------------------------------------

#include <string>
// ----------------------------------------------------------------------------
// FilePath
// ----------------------------------------------------------------------------
#pragma warning (disable:4251) // dll and STL

	struct FilePath
	{
		std::string mDirectory;
		std::string mExtension;
		std::string mFilename;
		std::string mFullPath;

		// CTOR
		FilePath();
		FilePath(const char * path);

		// PARSE OPERATION
		void ParsePath(const char * path = NULL);


		// Folder
		/*bool IsFolder() const;
		static bool Exists(const char * fullPath);
		static std::string GetCurrentDir();*/
	};
#pragma warning (default:4251) // dll and STL
// ----------------------------------------------------------------------------
#endif
