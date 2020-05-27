#include "ScreenCapture/ScreenCapture.hh"
#include "Input/InputManager.hh"
#include "OCR/OCR.hh"
#include "Color/Color.hh"
#include "Mouse/MouseUtilites.hh"
#include "Time/FrameRateController.hh"
#include "BMP/bitmap_image.hh"

#include <Windows.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

std::string GetActiveWindowTitle()
{
	char wnd_title[256];
	HWND hwnd = GetForegroundWindow(); // get handle of currently active window
	GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
	return wnd_title;
}

int main()
{
	std::string AppwindowName = "LOL Auto Smite";
	std::string lolWindowName = "League of Legends (TM) Client";
	SetConsoleTitle(AppwindowName.c_str());
	InputManager::Initialize();

	Position pos;
	Color posColor;

	while (1)
	{

		if (lolWindowName != GetActiveWindowTitle())
			continue;
			
		FrameRateController::StartFrame();
		InputManager::Update();

		//ScreenCapture::Instance().ScreenCap();

		/*if (InputManager::IsKeyPressed('C'))
		{
			auto img = ScreenCapture::Instance().ScreenRegionToImage(0, 0, 1920, 1080,false);
			img.save_image("test.bmp");
		}
		if (InputManager::IsKeyPressed('S'))
		{
			pos = MouseUtilities::GetMousePos();
			posColor = ScreenCapture::Instance().GetColor(pos.x, pos.y);
			std::cout << pos << posColor;
		}

		*/

		//if (InputManager::IsKeyDown(VK_SPACE))
		//{
			ScreenCapture::Instance().UseSmite();
		//}
		
		
		InputManager::EndUpdate();
		FrameRateController::EndFrame();


	}

	/*if (InputManager::IsKeyPressed(VK_DOWN))
	{
		++offsetY;
	}
	if (InputManager::IsKeyPressed(VK_UP))
	{
		--offsetY;
	}
	if (InputManager::IsKeyPressed(VK_LEFT))
	{
		--offsetX;
	}
	if (InputManager::IsKeyPressed(VK_RIGHT))
	{
		++offsetX;
	}*/
	//std::cout << FrameRateController::GetFps();
	
	return 0;
}