#include "Window.h"

namespace Window
{

	Window::Window(HWND handle, HINSTANCE instance, GLfloat width, GLfloat height, const char * title, GLint show)
		:mWindowHandle(handle), mWindowInstance(instance), wTitle(title), wWidth(width), wHeight(height), mfullscreen_handler(false)
	{
		createWindow();

		// Creating the window was successful -> Show the window
		ShowWindow(mWindowHandle, show);
		UpdateWindow(mWindowHandle);
	}
	Window::~Window()
	{
		destroyWindow();
	}

	void Window::setFullScreen()
	{
		mfullscreen_handler = !mfullscreen_handler;

		if (mfullscreen_handler)
		{
			HMONITOR hmon = MonitorFromWindow(mWindowHandle, MONITOR_DEFAULTTONEAREST);
			MONITORINFO mi = { sizeof(mi) };
			if (!GetMonitorInfo(hmon, &mi)) return;
			long w = mi.rcMonitor.right - mi.rcMonitor.left;
			long h = mi.rcMonitor.bottom - mi.rcMonitor.top;
			DWORD dwstyle = GetWindowLong(mWindowHandle, GWL_STYLE);
			dwstyle |= (WS_OVERLAPPED | WS_POPUP);
			dwstyle &= ~WS_OVERLAPPEDWINDOW;

			SetWindowLongPtr(mWindowHandle, GWL_STYLE, dwstyle);
			SetWindowPos(mWindowHandle
				, NULL
				, 0
				, 0
				, w
				, h
				, SWP_NOZORDER);

			wWidth = GLfloat(w);
			wHeight = GLfloat(h);
		}
		else {
			RECT winrect = { 0, 0, long(initialWidth), long(initialHeight) };
			AdjustWindowRect(&winrect, WS_OVERLAPPEDWINDOW, GL_FALSE);
			DWORD dwstyle = GetWindowLong(mWindowHandle, GWL_STYLE);
			dwstyle |= WS_OVERLAPPEDWINDOW;
			dwstyle &= ~(WS_OVERLAPPED | WS_POPUP);

			SetWindowLongPtr(mWindowHandle, GWL_STYLE, dwstyle);
			SetWindowPos(mWindowHandle
				, NULL
				, 0
				, 0
				, winrect.right - winrect.left
				, winrect.bottom - winrect.top
				, SWP_NOZORDER);

			wWidth = initialWidth;
			wHeight = initialHeight;
		}
	}

	void Window::SetResolution(GLfloat width, GLfloat height)
	{
		long w = (long)width;
		long h = (long)height;
		DWORD dwstyle = GetWindowLong(mWindowHandle, GWL_STYLE);
		dwstyle |= (WS_OVERLAPPED | WS_POPUP);
		dwstyle &= ~WS_OVERLAPPEDWINDOW;

		SetWindowLongPtr(mWindowHandle, GWL_STYLE, dwstyle);
		SetWindowPos(mWindowHandle
			, NULL
			, 0
			, 0
			, w
			, h
			, SWP_NOZORDER);

		wWidth = GLfloat(w);
		wHeight = GLfloat(h);
	}

	void Window::ChangeResolution(mResolution type)
	{
		switch (type)
		{
		case mSmall:
		{
			SetResolution(800, 600);
			break;
		}
		case mMedium:
		{
			SetResolution(1280, 720);
			break;
		}
		case mMediumPlus:
		{
			SetResolution(1366, 768);
			break;
		}
		case mFull:
		{
			setFullScreen();
			break;
		}
		default:
			break;
		}
	}

	void Window::createWindow()
	{
		RECT winrect = { 0, 0, long(wWidth), long(wHeight) };
		AdjustWindowRect(&winrect, WS_OVERLAPPEDWINDOW, GL_FALSE);

		mWindowHandle = CreateWindow(
			wTitle,
			wTitle,
			WS_OVERLAPPEDWINDOW,               //style of window.
			0,
			0,
			winrect.right - winrect.left,
			winrect.bottom - winrect.top,
			NULL,                             //Parent Window: wParentHandle(commented)
			NULL,                             //Handle to the menu
			mWindowInstance,
			NULL                             // Pointer to the value to be passed to the window when it's created and when it receives the WM_CREATE message. No need here.
		);

		//check if it worked
		if (!mWindowHandle)
		{
			MessageBox(NULL, "Error creating window", "ERROR!", NULL);
		}
	}

	void Window::destroyWindow()
	{
		if (mWindowHandle)
		{
			// WIN32 API
			::DestroyWindow(mWindowHandle);
			mWindowHandle = NULL;
		}
	}
}