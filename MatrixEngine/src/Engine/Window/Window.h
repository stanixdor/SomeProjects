#pragma once

#include "WindowManager.h"

namespace Window
{
	class Window
	{
		friend class WindowManager;
	public:
		enum mResolution {mSmall, mMedium, mMediumPlus, mFull};
		Window(){}
		Window(HWND handle, HINSTANCE instance, GLfloat width, GLfloat height, const char * title, GLint show);
		~Window();

		void setFullScreen();

		void ChangeResolution(mResolution type);
		void SetResolution(GLfloat width, GLfloat height);

        bool isFullScreen() { return mfullscreen_handler; }
		HWND getHandler() 
		{ 
			return mWindowHandle; 
		}
		GLfloat getWidth() { return wWidth; }
		GLfloat getHeight() { return wHeight; }

		void createWindow();
		void destroyWindow();

	private:
		HWND mWindowHandle;
		HINSTANCE  mWindowInstance;

		const char * wTitle;
		GLfloat wWidth, wHeight;
		GLfloat oldWidth, oldHeight;
		GLfloat initialWidth = 1280;
		GLfloat initialHeight = 720;


		bool mfullscreen_handler;
	};
}
