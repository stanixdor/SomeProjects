#pragma once
#include "../Platform/Input/Input.h"
#include "../Platform/Time/FrameRateController.h"
#include "EngineDebug/MyDebug.h"
#include "../Window/WindowManager.h"
#include <Windows.h>
#include "../Window/Window.h"
#include "../Audio/Audio.h"
#include "../Graphics/GraphicsSystem.h"
#include "../Logic/Level.h"
#include "../../Game/Levels/LevelTest.h"
#include "../../Game/Levels/MenuLevel.h"
#include "RegisterAllComponent.h"
#include "Editor\ImGuiUpdate.h"
#include "Platform\MessageWindow\MessageWindow.h"
#include "Platform\Resource Manager\ResourceManager.h"
#include "EngineDebug\ConsolePrinting.h"
#include "Physics\CollisionSystem.h"
#include "Components\Pipe.h"
#include "Levels\GenericLevel.h"
#include "Physics\CollisionTable.h"
#include <sstream>

Window::Window *the_window_to_use;
xboxController* controller;
bool fullscreenHack = false;



void endOfFrameUpdate()
{
	objectManager.LateUpdate();
	GraphicsSystem::Instance()->endOfFrameUpdate();
	CollisionSystem::Instance()->endOfFrameUpdate();
	EngineImGUI::ImGuiLateUpdate();
	
	//auto Player_ = objectManager.FindObjectByName("player");
	//auto Head_ = objectManager.FindObjectByName("PlayerHead");
	//
	//if (Player_ != nullptr && Head_ != nullptr)
	//{
	//	Head_->m_transform->mPosition = Player_->m_transform->mPosition;
	//}
}

void MatrixInitialize(HINSTANCE hInstance, int nCmdShow)
{
	//allocate a console
	ShowConsole();
	RegisterAllComponents();

	Window::WindowManager::Instance()->Initialize("DIBO Jones", 1280, 720, nCmdShow, hInstance);
	the_window_to_use = Window::WindowManager::Instance()->GetWindow();
	GraphicsSystem::Instance()->Initialize(the_window_to_use->getHandler());
	CollisionTable::Instance()->Initialize();
	ResourceManager::RefreshResources();
    
	/*Levels["MenuLevel"] = std::make_unique<MenuLevel>();
	Levels["LevelTest"] = std::make_unique<LevelTest>();*/
	Levels["GenericLevel"] = std::make_unique<LevelTest>();
	currentLevel = Levels["GenericLevel"].get();
	ResetLevel("FinalLevelWeek5.level", "Data/Levels/");
	EngineImGUI::initialize();

	Input::Instance()->Initialize();

	//initialize FrameRateController
	FRC::FrameRateController::Instance()->Initialize();
	getControllers();
	controller = getFreeController();

	gAudioMgr.Initialize();
	Console::WriteLine(std::stringstream("Matrix Engine initialized correctly"));
	//if(playMode)
		//ResetLevel("First.level", "Data/Levels/");
	
}
void MatrixEnd()
{
	HideConsole();
	gAudioMgr.StopAll();
	gAudioMgr.Shutdown();
}

int MatrixUpdate()
{
	FRC::FrameRateController::Instance()->Update();
	controllerInput();
	Input::Instance()->Update();
	Window::WindowManager::Instance()->WindowManagerUpdate();


	if (playMode && Input::Instance()->KeyTriggered('J'))
	{
		Pipe::fakeTongueHasFullLength = !Pipe::fakeTongueHasFullLength;
	}
	currentLevel->Update();

    if (!fullscreenHack)
    {
		the_window_to_use->setFullScreen();
		Window::WindowManager::Instance()->SetWidthHeight(the_window_to_use->getWidth(), the_window_to_use->getHeight());
		GraphicsSystem::Instance()->UpdateCameras();

        fullscreenHack = true;
    }

	if (playMode && Input::Instance()->KeyTriggered('T'))
		thomasMode = !thomasMode;

	if (controller->buttonIsTriggered(Buttons::A))
		std::cout << "Pressed A in the Xbox Controller" << std::endl;

	if (Input::Instance()->KeyTriggered('O') && playMode)
	{
		PAUSE = !PAUSE;
	}

	if (Input::Instance()->KeyPressed(Input::eControl) && Input::Instance()->KeyTriggered('P'))
		playMode = !playMode;


	if (Input::Instance()->KeyPressed(Input::Keys::eShift) && Input::Instance()->KeyTriggered('F'))
	{
		the_window_to_use->setFullScreen();
		Window::WindowManager::Instance()->SetWidthHeight(the_window_to_use->getWidth(), the_window_to_use->getHeight());
		GraphicsSystem::Instance()->UpdateCameras();
	}
	/*if (Input::Instance()->KeyTriggered('C'))
	{
		the_window_to_use->ChangeResolution(Window::Window::mMedium);
		Window::WindowManager::Instance()->SetWidthHeight(the_window_to_use->getWidth(), the_window_to_use->getHeight());
		GraphicsSystem::Instance()->UpdateCameras();
	}
	if (Input::Instance()->KeyTriggered('V'))
	{
		the_window_to_use->ChangeResolution(Window::Window::mMediumPlus);
		Window::WindowManager::Instance()->SetWidthHeight(the_window_to_use->getWidth(), the_window_to_use->getHeight());
		GraphicsSystem::Instance()->UpdateCameras();
	}
	if (Input::Instance()->KeyTriggered('B'))
	{
		the_window_to_use->ChangeResolution(Window::Window::mSmall);
		Window::WindowManager::Instance()->SetWidthHeight(the_window_to_use->getWidth(), the_window_to_use->getHeight());
		GraphicsSystem::Instance()->UpdateCameras();
	}*/
	
	//close the game with Escape
    if (Input::Instance()->KeyPressed(Input::eShift) && Input::Instance()->KeyTriggered(Input::Keys::eEsc))
    {
        Input::Instance()->removeKeyPress(Input::Keys::eEsc);

        if (the_window_to_use->isFullScreen())
        {
            the_window_to_use->setFullScreen();
            Window::WindowManager::Instance()->SetWidthHeight(the_window_to_use->getWidth(), the_window_to_use->getHeight());
            GraphicsSystem::Instance()->UpdateCameras();
        }
        int messageBoxAnswer = MessageWindow::warningMessage("Leaving the program", "Are you sure you want to close the game?");

        if (messageBoxAnswer == IDYES)
        {
            HideConsole();
            return 0;
        }

        if (!the_window_to_use->isFullScreen())
        {
            the_window_to_use->setFullScreen();
            Window::WindowManager::Instance()->SetWidthHeight(the_window_to_use->getWidth(), the_window_to_use->getHeight());
            GraphicsSystem::Instance()->UpdateCameras();
        }
    }

	if (!PAUSE)
	{
		
		CollisionSystem::Instance()->Update();
		
	}
	objectManager.Update();
	GraphicsSystem::Instance()->Update();
	endOfFrameUpdate();
	objectManager.DestroyDeathObjects();
	FRC::FrameRateController::Instance()->EndFrame();

    return 1;
}