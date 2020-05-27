#include "WindowManager.h"
#include "../Platform/Input/Input.h"
#include "Window.h"
#include "Graphics\GraphicsSystem.h"


//Cosas a mirar
// 1.-La window existe? -->mirar ok
// 2.-WindowManager destructor. ok




//
namespace Window
{
	bool WindowManager::gWindowExists = false;
	HINSTANCE WindowManager::wmAppInstance = NULL;

	WindowManager::~WindowManager()
	{
		UnregisterWindow();
	}

	void WindowManager::Initialize(const char *windowTitle, GLfloat windowWidth, GLfloat windowHeight, GLint show, HINSTANCE hinstance)
	{
		wmhinstance = GetModuleHandleA(NULL);
		wExists = true;
		wTitle = windowTitle;
		wWidth = windowWidth;
		wHeight = windowHeight;

		wInstance = hinstance;

		//Initialize window.
		WindowManagerInit();

		winInUsage = new Window(wWindowHandle, wmhinstance, wWidth, wHeight, wTitle, show);

		wmAppInstance = wInstance;
	}

	void WindowManager::WindowManagerInit()
	{
		wmWindowClass.cbSize = sizeof(WNDCLASSEX);
		wmWindowClass.style = CS_HREDRAW | CS_VREDRAW;
		wmWindowClass.lpfnWndProc = MessageHandler;
		wmWindowClass.cbClsExtra = 0;
		wmWindowClass.cbWndExtra = 0;
		wmWindowClass.hInstance = wmhinstance; // GetModuleHandleA(NULL);
		wmWindowClass.hIcon = NULL;
		wmWindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
		wmWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wmWindowClass.lpszMenuName = NULL;
		wmWindowClass.hIconSm = NULL;
		wmWindowClass.lpszClassName = wTitle;

		if (!RegisterClassExA(&wmWindowClass))
			MessageBox(NULL, "Call to RegisterClassEx Failed", "CS230", NULL);
	}

	void WindowManager::Update()
	{
		RECT sizeOfWindow;
		Window* mCurrentWindow = GetWindow();

		if (GetWindowRect(mCurrentWindow->getHandler(), &sizeOfWindow))
			return;

		GLfloat height = static_cast<GLfloat>(sizeOfWindow.bottom - sizeOfWindow.top);
		GLfloat width =  static_cast<GLfloat>(sizeOfWindow.right - sizeOfWindow.left);

		if (height != mCurrentWindow->getHeight() || width != mCurrentWindow->getWidth())
		{
			mCurrentWindow->SetResolution(width, height);

			GraphicsSystem::Instance()->UpdateCameras();
		}

	}

	void WindowManager::createWindow()
	{
		RECT winrect = { 0, 0, long(wWidth), long(wHeight) };
		AdjustWindowRect(&winrect, WS_OVERLAPPEDWINDOW, GL_FALSE);

		wWindowHandle = CreateWindow(
			wTitle,
			wTitle,
			WS_OVERLAPPEDWINDOW,               //style of window.
			0,
			0,
			winrect.right - winrect.left,
			winrect.bottom - winrect.top,
			NULL,                             //Parent Window: wParentHandle(commented)
			NULL,                             //Handle to the menu
			wInstance,
			NULL                             // Pointer to the value to be passed to the window when it's created and when it receives the WM_CREATE message. No need here.
		);

		//check if it worked
		if (!wWindowHandle)
		{
			MessageBox(NULL, "Error creating window", "ERROR!", NULL);
		}

	}

	void WindowManager::WindowManagerUpdate()
	{
		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				wExists = false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void WindowManager::UnregisterWindow()
	{
		UnregisterClass(wmWindowClass.lpszClassName, wmAppInstance);

		wExists = false;
	}

	HWND WindowManager::getHandler()
	{
		return wWindowHandle;
	}

	GLfloat WindowManager::getWidht()
	{
		return wWidth;
	}

	GLfloat WindowManager::getHeight()
	{
		return wHeight;
	}

	bool WindowManager::WindowExists() const
	{
		return wExists;
	}

	LRESULT CALLBACK WindowManager::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		Input* input = Input::Instance();
		// Handle the input message here. 
		input->HandleWin32Message(msg, wParam, lParam);

		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			gWindowExists = false;
			break;
		case WM_CREATE:
			break;
			/* called any time the window is moved */
		case WM_MOVE:
			/* Invalidate the rect to force a redraw */
			InvalidateRect(hWnd, NULL, FALSE);
			// When we don't handle a message. We call the default window procedure. This will
			// ensure that every message is processed.
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
		}

		return 0;
	}
}

