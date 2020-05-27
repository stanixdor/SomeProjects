#include "../MatrixEngine.h"
#include "../Game/Components/EventTest.h" //component that recieves events
#include <Windows.h>
#include "../Engine/Logic/Level.h"
#include "../Engine/Core/System.h"
#include "Editor\ImGuiFunc.h"
#include "imgui\imgui.h"
#include "Graphics\GraphicsSystem.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE,
	_In_ LPSTR,
	_In_ int       nCmdShow
	)
{

	MatrixInitialize(hInstance, nCmdShow);

	//Sound *			CallOnMe;
	//Voice*          SongInstance;

	ImVec4 clear_color = ImColor(114, 144, 154);	//Background color for the engine

	//gAudioMgr.CreateSound("..\\data\\CallOnMe.mp3");
	//CallOnMe = gAudioMgr.CreateSound("src\\Game\\data\\CallOnMe.mp3");
	//SongInstance = gAudioMgr.Play(CallOnMe);
	int returnValue = 1;

	ImGui_Init(Window::WindowManager::Instance()->GetWindow());

	while (returnValue)
	{
		returnValue = MatrixUpdate();

		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	}
	MatrixEnd();

	return 0;
}
