// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEInput.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	declaration for input stuff
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#pragma once
// ---------------------------------------------------------------------------
// Defines/Enums


#define INPUT_KEY_NUM	256
#define INPUT_MOUSE_NUM 3

#include "../GLM/glm.hpp"
#include "xboxController.h"
#include "Utils/ISingleton.h"
#include "Core/DataTypes.h"
#include <Windows.h>
#include <vector>

#pragma warning (disable:4251) // dll and STL

// ---------------------------------------------------------------------------
// class definitions
class Input : public ISingleton<Input>
{

public:
	enum Keys
	{
		eAny = -1,
		eMouseLeft = 0,
		eMouseRight = 1,
		eMouseMiddle = 2,
		eControl = 17,
		eShift = 16,
		eTab = 9,
		eAlt = 10,
		eEsc = 27,
		eBackspace = 8,
		eDelete = 127
	};

public:

	// ISystem

	virtual bool Initialize();
	virtual void Update();
	virtual ~Input();

	// Input
    void fakeKeyPress(int key);
    void removeKeyPress(int key);
	bool HandleWin32Message(UINT msg, WPARAM wParam, LPARAM lParam);
	bool AnyKey();
	bool AnyKeyTriggered();
	bool KeyPressed(int key);
	bool KeyTriggered(int key);
	bool MousePressed(int button);
	bool MouseTriggered(int button);
	int convertKeyboardInput(WPARAM wParam);

	//AEVec2 GetMousePos();
	//AEVec2 GetPrevMousePos();
	//AEVec2 GetMouseMovement();
	f32		GetMouseWheel();
	//bool MouseInWindow();

	const std::vector<u8> & AllKeyPressed();
	const std::vector<u8> & AllKeyTriggered();

	//get mouse position in world coordinates.
	glm::vec2 ScreenToWorld();
	glm::vec2 ScreenToWorld(POINT& mPos);
	glm::vec2 GetMouseScreen();

private:

	u8	mKeyCurr[INPUT_KEY_NUM];
	u8	mKeyPrev[INPUT_KEY_NUM];
	u8	mMouseCurr[INPUT_MOUSE_NUM];
	u8	mMousePrev[INPUT_MOUSE_NUM];
	f32 mMouseWheel;
	//AEVec2 mMousePos;			// Mouse Position in Centered Coordinates
	//AEVec2 mMousePrevPos;		// Previous Mouse Position in Centered Coordinates
	//AEVec2 mMouseDiff;		// Mouse movement this update: Prev - Curr
	bool		 mMouseInWindow;	// Specifies whether the mouse is contained insisde the window


	bool				mAnyKey;	 // Any key is pressed. 
	bool				mAnyKeyTriggered;	 // Any key is pressed. 
	std::vector<u8>	mKeyPressed; // keys pressed last frame.
	std::vector<u8>	mKeyTriggered; // keys pressed last frame.

};

#pragma warning (default:4251) // dll and STL

// ---------------------------------------------------------------------------

