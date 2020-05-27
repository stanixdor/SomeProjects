#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>


class KeyboardUtilities
{
public:
	static void PressKey(char key)
	{
		keybd_event(key, static_cast<BYTE>(MapVirtualKey(key, 0)), 0, 0);
		keybd_event(key, static_cast<BYTE>(MapVirtualKey(key, 0)), KEYEVENTF_KEYUP, 0);
	}
	static void HoldKey(char key)
	{
		auto searchResult = std::find(KeyboardUtilities::holdKeys.begin(), KeyboardUtilities::holdKeys.end(), key);
		
		if (searchResult != std::end(KeyboardUtilities::holdKeys))
		{
			//intentando mantener una tecla ya presionada
			return;
		}
		
		keybd_event(key, static_cast<BYTE>(MapVirtualKey(key, 0)), 0, 0);
		KeyboardUtilities::holdKeys.push_back(key);
	}
	static void ReleaseKey(char key)
	{
		auto searchResult = std::find(KeyboardUtilities::holdKeys.begin(), KeyboardUtilities::holdKeys.end(), key);

		if (searchResult != std::end(KeyboardUtilities::holdKeys))
		{
			keybd_event(key, static_cast<BYTE>(MapVirtualKey(key, 0)), KEYEVENTF_KEYUP, 0);

			for (auto it = KeyboardUtilities::holdKeys.begin(); it != KeyboardUtilities::holdKeys.end(); ) {
				if (*it == key) 
				{
					it = KeyboardUtilities::holdKeys.erase(it);
				}
				else 
				{
					++it;
				}
			}
			return;
		}
		//trying to release a non existant key
	}
	static void ReleaseAllKeys()
	{
		for (const char key : KeyboardUtilities::holdKeys)
			keybd_event(key, static_cast<BYTE>(MapVirtualKey(key, 0)), KEYEVENTF_KEYUP, 0);
		KeyboardUtilities::holdKeys.clear();
	}
private:
	static std::vector<char> holdKeys;
};