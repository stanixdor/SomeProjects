#include <iostream>
#include <fstream>
#include <sstream>

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <shlobj.h>
#include <vector>
#include <string>
#include <algorithm>
#include <urlmon.h>                 //Needed for the URLDownloadToFile() function
#pragma comment(lib, "urlmon.lib")  //Needed for the URLDownloadToFile() function


std::string folderPath;
std::string filePath;
std::vector<std::string> clipSlugs;

class MyCallback : public IBindStatusCallback
{
public:
	MyCallback() {}

	~MyCallback() { }

	// This one is called by URLDownloadToFile
	STDMETHOD(OnProgress)(/* [in] */ ULONG ulProgress, /* [in] */ ULONG ulProgressMax, /* [in] */ ULONG ulStatusCode, /* [in] */ LPCWSTR wszStatusText)
	{
		std::cout << "Downloaded " << ulProgress << " of " << ulProgressMax << " byte(s), " << " Status Code = " << ulStatusCode << std::endl;
		return S_OK;
	}

	// The rest  don't do anything...
	STDMETHOD(OnStartBinding)(/* [in] */ DWORD dwReserved, /* [in] */ IBinding __RPC_FAR* pib)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetPriority)(/* [out] */ LONG __RPC_FAR* pnPriority)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnLowResource)(/* [in] */ DWORD reserved)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnStopBinding)(/* [in] */ HRESULT hresult, /* [unique][in] */ LPCWSTR szError)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetBindInfo)(/* [out] */ DWORD __RPC_FAR* grfBINDF, /* [unique][out][in] */ BINDINFO __RPC_FAR* pbindinfo)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnDataAvailable)(/* [in] */ DWORD grfBSCF, /* [in] */ DWORD dwSize, /* [in] */ FORMATETC __RPC_FAR* pformatetc, /* [in] */ STGMEDIUM __RPC_FAR* pstgmed)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(OnObjectAvailable)(/* [in] */ REFIID riid, /* [iid_is][in] */ IUnknown __RPC_FAR* punk)
	{
		return E_NOTIMPL;
	}

	// IUnknown stuff
	STDMETHOD_(ULONG, AddRef)()
	{
		return 0;
	}

	STDMETHOD_(ULONG, Release)()
	{
		return 0;
	}

	STDMETHOD(QueryInterface)(/* [in] */ REFIID riid, /* [iid_is][out] */ void __RPC_FAR* __RPC_FAR* ppvObject)
	{
		return E_NOTIMPL;
	}
};

bool GetFolder(std::string& folderpath, HWND hOwner = NULL)
{
	bool retVal = false;

	// The BROWSEINFO struct tells the shell 
	// how it should display the dialog.
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.ulFlags = BIF_USENEWUI;
	bi.hwndOwner = hOwner;
	bi.lpszTitle = "Select clip download destination folder";

	// must call this if using BIF_USENEWUI
	::OleInitialize(NULL);

	// Show the dialog and get the itemIDList for the 
	// selected folder.
	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

	if (pIDL != NULL)
	{
		// Create a buffer to store the path, then 
		// get the path.
		char buffer[_MAX_PATH] = { '\0' };
		if (::SHGetPathFromIDList(pIDL, buffer) != 0)
		{
			// Set the string value.
			folderpath = buffer;
			retVal = true;
		}

		// free the item id list
		CoTaskMemFree(pIDL);
	}

	::OleUninitialize();

	return retVal;
}

bool GetFileName(std::string& filepath)
{
	char filename[MAX_PATH] = { '\0' };

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select your text file with Twitch clips";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		//std::cout << "You chose the file \"" << filename << "\"\n";
		filepath = filename;
		return true;
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
		return false;
	}
}

void eraseSubStr(std::string& mainStr, const std::string& toErase)
{
	// Search for the substring in string
	size_t pos = mainStr.find(toErase);

	if (pos != std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}

bool CheckLegalFileName(std::string& sToCheck)
{

	std::wstring stemp = std::wstring(sToCheck.begin(), sToCheck.end());
	LPCWSTR filename = stemp.c_str();

	WCHAR valid_invalid[MAX_PATH];
	wcscpy_s(valid_invalid, filename);

	int result = PathCleanupSpec(nullptr, valid_invalid);

	std::wstring fixed(valid_invalid);
	std::string str(fixed.begin(), fixed.end());
	sToCheck = str;

	// If return value is non-zero, or if 'valid_invalid' 
	// is modified, file-name is assumed invalid
	return result == 0 && _wcsicmp(valid_invalid, filename) == 0;
}

int main()
{
	if (!GetFileName(filePath))
		return -1;

	if (!GetFolder(folderPath))
		return -1;

	std::ifstream infile(filePath); 
	std::string line;
	std::string toErase{ "https://clips.twitch.tv/" };

	if (infile.is_open())
	{
		while (std::getline(infile, line))
		{
			eraseSubStr(line, toErase);
			clipSlugs.push_back(line);
		}
		infile.close();
	}
	
	for (const auto& slug : clipSlugs)
	{
		std::string p1{ "curl -H \"Client-ID: f8jcgpgdc51lsqvkvcmh6djpbxo26b\" \\ -X GET \"https://api.twitch.tv/helix/clips?id=" };
		std::string p2{ "\" > temp" };
		std::string final = p1 + slug + p2;
		const char* t = final.c_str();
		system(t);

		std::ifstream tempfile("temp");
		std::stringstream buffer;
		buffer << tempfile.rdbuf();
		std::string fileContent = buffer.str();
		std::string clipTitle;
		std::string t1{ "\"title\":\"" };
		std::string t2{ "\",\"view_count" };
		std::string url;
		std::string t3{ "https://clips-media-assets2.twitch.tv/" };
		std::string t4{ "-preview-" };
		if (tempfile.is_open())
		{
			std::size_t title1 = fileContent.find(t1);
			std::size_t title2 = fileContent.find(t2);
			title1 += t1.length();
			std::size_t length = title2 - title1;
			if (title1 != std::string::npos && title2 != std::string::npos)
			{
				clipTitle = fileContent.substr(title1, length);
				CheckLegalFileName(clipTitle);
				//clipTitle.erase(std::remove(clipTitle.begin(), clipTitle.end(), '\\n'), clipTitle.end());
			}

			std::size_t title3 = fileContent.find(t3);
			std::size_t title4 = fileContent.find(t4);
			std::size_t length2 = title4 - title3;

			if (title3 != std::string::npos && title4 != std::string::npos)
			{
				url = fileContent.substr(title3, length2) + ".mp4";
			}

			tempfile.close();

			if (std::remove("temp") != 0) // delete file
				std::cout << "error deleting temp file";

			//std::string url = "https://clipr.xyz/AgileTemperedStinkbugTBTacoRight";

			//Convert the string to a LPCSTR type so we can use it in the URLDownloadToFile() function
			LPCSTR lpcURL = url.c_str();

			std::string destination = folderPath + "\\" + clipTitle + ".mp4";

			//Convert the string to a LPCSTR type so we can use it in the URLDownloadToFile() function
			LPCSTR lpcDestination = destination.c_str();
			MyCallback pCallback;
			HRESULT hr = URLDownloadToFile(NULL, lpcURL, lpcDestination, 0, &pCallback);

			if (hr == S_OK)
			{
				printf("Ok\n");
			}
			else if (hr == E_OUTOFMEMORY)
			{
				printf("Buffer length invalid, or insufficient memory\n");
			}
			else if (hr == INET_E_DOWNLOAD_FAILURE)
			{
				printf("URL is invalid\n");
			}
			else {
				printf("Other error: %d\n", hr);
			}
		}

	}	

	return 0;
}
