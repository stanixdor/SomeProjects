// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEInput.cpp
// Author			:	Sun Tjen Fam, Antoine Abi Chakra
// Creation Date	:	2008/0131
// Purpose			:	Input wrapper
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------
#include "Input.h"
#include "Window/Window.h"
#include "Graphics\Camera.h"
#include "Graphics\GraphicsSystem.h"
#include "Transform\Transform2D.h"
#include "Platform\Input\Input Events\MouseClickEvent.h"
#include "Platform\EventSystem\event_dispatcher.h"
#include <cctype>
#include <stdlib.h>

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
Input::~Input()
{}

// ---------------------------------------------------------------------------
bool Input::Initialize()
{
	std::memset(mKeyCurr, 0, sizeof(u8) * INPUT_KEY_NUM);
	std::memset(mKeyPrev, 0, sizeof(u8) * INPUT_KEY_NUM);

	std::memset(mMouseCurr, 0, sizeof(u8) * INPUT_MOUSE_NUM);
	std::memset(mMousePrev, 0, sizeof(u8) * INPUT_MOUSE_NUM);

	mAnyKey = false;
	mAnyKeyTriggered = false;
	mKeyPressed.clear();
	mKeyTriggered.clear();
	mMouseWheel = 0.0f;
	return true;
}

void Input::fakeKeyPress(int key)
{
    mKeyCurr[key] = true;
}

void Input::removeKeyPress(int key)
{
    mKeyCurr[key] = false;
}


// ---------------------------------------------------------------------------
namespace internals
{
bool mouseWheelThisFrame = false;
} 

using namespace internals;
bool Input::HandleWin32Message(UINT msg, WPARAM wParam, LPARAM lParam)
{
	ENGINE_UNUSED(lParam);
	switch (msg)
	{
		// Keyboard
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			mKeyCurr[convertKeyboardInput(wParam)] = 1;
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			mKeyCurr[convertKeyboardInput(wParam)] = 0;
			break;

			// Mouse Right Button		
		case WM_RBUTTONDOWN:
			mMouseCurr[Input::eMouseRight] = 1;
			break;

		case WM_RBUTTONUP:
			mMouseCurr[Input::eMouseRight] = 0;
			break;
			// Mouse Left Button
		case WM_LBUTTONDOWN:
			mMouseCurr[Input::eMouseLeft] = 1;
			break;

		case WM_LBUTTONUP:
			mMouseCurr[Input::eMouseLeft] = 0;
			break;
			// Mouse Left Button
		case WM_MBUTTONDOWN:
			mMouseCurr[Input::eMouseMiddle] = 1;
			break;

		case WM_MBUTTONUP:
			mMouseCurr[Input::eMouseMiddle] = 0;
			break;
			// not an input message, return false
		case WM_MOUSEWHEEL:
		{
			short  delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mMouseWheel = (f32)delta / 120.0f;
			mouseWheelThisFrame = true;
			break;
		}
		default:
			return false;
	}
	return true;
}

// ---------------------------------------------------------------------------

void Input::Update()
{
	int i;		// counter for loop
//	POINT mp;	// to store the mouse position

	// reset the mouse wheel
	if (mouseWheelThisFrame)
		mouseWheelThisFrame = false;
	else
		mMouseWheel = 0.0f;

	// Clear the keys that were pressed last frame
	mKeyPressed.clear();
	mKeyTriggered.clear();
	mAnyKey = false;
	mAnyKeyTriggered = false;

	// Determine whether a key was triggered or not
	for (i = 0; i < INPUT_KEY_NUM; ++i)
	{
		if (mKeyCurr[i])
		{
			// any keys has been pressed
			mAnyKey = true;
			mKeyPressed.push_back((u8)i);

			if (!mKeyPrev[i])
			{
				mAnyKeyTriggered = true;
				mKeyTriggered.push_back((u8)i);
				mKeyCurr[i] = 2;
			}
			else
				mKeyCurr[i] = 1;
		}
		mKeyPrev[i] = mKeyCurr[i];
	}

	//Determine whether a mouse button was triggered or not
	for (i = 0; i < INPUT_MOUSE_NUM; ++i)
	{
		if (mMouseCurr[i])
		{
			if (!mMousePrev[i])
			{
				mMouseCurr[i] = 2;


			}
			else
			{

				mMouseCurr[i] = 1;
			}
		}
		//if the mouse was released this frame
		if (mMousePrev[0] && (mMouseCurr[0] == 0) )
			if(playMode)
				Messaging::EventDispatcher::get_instance().trigger_event(MouseReleaseEvent(ScreenToWorld()));

		mMousePrev[i] = mMouseCurr[i];
	}

	


	if (mMouseCurr[0])
	{
		if (playMode)
			Messaging::EventDispatcher::get_instance().trigger_event(MouseClickEvent(ScreenToWorld()));
	}
}

// ---------------------------------------------------------------------------

const std::vector<u8> & Input::AllKeyPressed()
{
	return mKeyPressed;
}

// ---------------------------------------------------------------------------

const std::vector<u8> & Input::AllKeyTriggered()
{
	return mKeyTriggered;
}

glm::vec2 Input::ScreenToWorld()
{
	POINT mPos;
	GetCursorPos(&mPos);

	Window::Window * mainWindow = Window::WindowManager::Instance()->GetWindow();
	Graphics::Camera * camera = GraphicsSystem::Instance()->mSpriteLayer[0]->mCameraLayer;

	if (camera == nullptr)
		return {0.f ,0.f};
	ScreenToClient(mainWindow->getHandler(), &mPos);

	glm::vec2 mousePosVec;

	//mouse pos in windows coordinates.
	mousePosVec.x = (GLfloat)mPos.x;
	mousePosVec.y = (GLfloat)mPos.y;

	GLfloat viewport_width = camera->mViewport.GetWidth();
	GLfloat viewport_height = camera->mViewport.GetHeight();
	GLfloat window_width = Window::WindowManager::Instance()->getWidht();
	GLfloat window_height = Window::WindowManager::Instance()->getHeight();

	GLfloat ratio_w = window_width / viewport_width;
	GLfloat ratio_h = window_height / viewport_height;

	glm::vec2 VP_pos = camera->mViewport.GetPos();

	VP_pos.x -= window_width / 2.0f;
	VP_pos.y = VP_pos.y + window_height / 2.0f;

	VP_pos.x += viewport_width / 2.0f;
	VP_pos.y -= viewport_height / 2.0f;

	//point in viewport.(Top-left coordinates)
	glm::vec2 point_VP((mousePosVec.x - VP_pos.x) * ratio_w, (mousePosVec.y - VP_pos.y) * ratio_h);

	glm::vec2 cameraScale = camera->GetScale();

	GLfloat ratio_cam_width = cameraScale.x / viewport_width;
	GLfloat ratio_cam_height = cameraScale.y / viewport_height;

	//point in camera
	auto poscam = camera->CameraPos();

	//performing the scale of the camera
	point_VP.x = point_VP.x * ratio_cam_width;
	point_VP.y = point_VP.y * ratio_cam_height;
	//performing the translatino of the camera.
	point_VP.x += poscam.x;
	point_VP.y -= poscam.y;
	//top left coordintes.

	//do on centered coordinates.
	point_VP.x -= cameraScale.x / 2.0f;
	point_VP.y -= cameraScale.y / 2.0f;
	point_VP.y = -(point_VP.y);

	return point_VP;
}

glm::vec2 Input::ScreenToWorld(POINT & mPos)
{
	Window::Window * mainWindow = Window::WindowManager::Instance()->GetWindow();
	Graphics::Camera * camera = GraphicsSystem::Instance()->mSpriteLayer[0]->mCameraLayer;

	ScreenToClient(mainWindow->getHandler(), &mPos);

	glm::vec2 mousePosVec;

	//mouse pos in windows coordinates.
	mousePosVec.x = (GLfloat)mPos.x;
	mousePosVec.y = (GLfloat)mPos.y;

	GLfloat viewport_width = camera->mViewport.GetWidth();
	GLfloat viewport_height = camera->mViewport.GetHeight();
	GLfloat window_width = Window::WindowManager::Instance()->getWidht();
	GLfloat window_height = Window::WindowManager::Instance()->getHeight();

	GLfloat ratio_w = window_width / viewport_width;
	GLfloat ratio_h = window_height / viewport_height;

	glm::vec2 VP_pos = camera->mViewport.GetPos();

	VP_pos.x -= window_width / 2.0f;
	VP_pos.y = VP_pos.y + window_height / 2.0f;

	VP_pos.x += viewport_width / 2.0f;
	VP_pos.y -= viewport_height / 2.0f;

	//point in viewport.(Top-left coordinates)
	glm::vec2 point_VP((mousePosVec.x - VP_pos.x) * ratio_w, (mousePosVec.y - VP_pos.y) * ratio_h);

	glm::vec2 cameraScale = camera->GetScale();

	GLfloat ratio_cam_width = cameraScale.x / viewport_width;
	GLfloat ratio_cam_height = cameraScale.y / viewport_height;

	//point in camera
	glm::vec2 poscam;
	if(camera->mPlayer != nullptr)
		poscam = camera->mPlayer->GetPos();
	else poscam = camera->CameraPos();

	//performing the scale of the camera
	point_VP.x = point_VP.x * ratio_cam_width;
	point_VP.y = point_VP.y * ratio_cam_height;
	//performing the translatino of the camera.
	point_VP.x += poscam.x;
	point_VP.y -= poscam.y;
	//top left coordintes.

	//do on centered coordinates.
	point_VP.x -= cameraScale.x / 2.0f;
	point_VP.y -= cameraScale.y / 2.0f;
	point_VP.y = -(point_VP.y);

	return point_VP;
}

glm::vec2 Input::GetMouseScreen()
{
	POINT mPos;
	GetCursorPos(&mPos);

	Window::Window * mainWindow = Window::WindowManager::Instance()->GetWindow();

	ScreenToClient(mainWindow->getHandler(), &mPos);
	glm::vec2 mousePosVec;
	//mouse pos in windows coordinates.
	mousePosVec.x = (GLfloat)mPos.x;
	mousePosVec.y = (GLfloat)mPos.y;

	//std::cout << mousePosVec.x << "  " << mousePosVec.y << std::endl;

	//glm::vec4 pos = mViewport.getViewportMatrix() * glm::vec4(mousePosVec.x, mousePosVec.y, .0f, 1.0f);

	//std::cout << pos.x << "  " << pos.y << std::endl;

	return glm::vec2(mousePosVec);
}


// ---------------------------------------------------------------------------

bool Input::AnyKey()
{
	return mAnyKey;
}

// ---------------------------------------------------------------------------

bool Input::AnyKeyTriggered()
{
	return mAnyKeyTriggered;
}

// ---------------------------------------------------------------------------

bool Input::KeyPressed(int key)
{
	int trueKey = toupper(key);
	if (trueKey > INPUT_KEY_NUM - 1)
		return 0;

	if (trueKey == eAny)
		return mAnyKey;

	return mKeyCurr[trueKey] > 0;
}

// ---------------------------------------------------------------------------

bool Input::KeyTriggered(int key)
{
	int trueKey = toupper(key);
	if (trueKey > INPUT_KEY_NUM - 1)
		return 0;

	if (trueKey == eAny)
		return mAnyKey;
	return mKeyCurr[trueKey] == 2;
}

// ---------------------------------------------------------------------------

bool Input::MousePressed(int button)
{
	if (button > Input::eMouseMiddle)
		return 0;

	return mMouseCurr[button] > 0;
}

// ---------------------------------------------------------------------------

bool Input::MouseTriggered(int button)
{
	if (button > Input::eMouseMiddle)
		return 0;

	
	return mMouseCurr[button] == 2;
}

//get the codes to add characters here: https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
int Input::convertKeyboardInput(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_MENU:
		return Input::eAlt;

	case VK_SUBTRACT:
	case VK_OEM_MINUS:
		return '-';

	case VK_NUMPAD0:
		return '0';

	case VK_NUMPAD1:
		return '1';

	case VK_NUMPAD2:
		return '2';

	case VK_NUMPAD3:
		return '3';

	case VK_NUMPAD4:
		return '4';

	case VK_NUMPAD5:
		return '5';

	case VK_NUMPAD6:
		return '6';

	case VK_NUMPAD7:
		return '7';

	case VK_NUMPAD8:
		return '8';

	case VK_NUMPAD9:
		return '9';
	
	case VK_DECIMAL:
	case VK_OEM_PERIOD:
		return '.';

	case VK_DELETE:
		return 127;

	default:
		return static_cast<int>(wParam);
	}
}

f32		Input::GetMouseWheel()
{
	return mMouseWheel;
}
