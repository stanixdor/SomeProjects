//Mikel Fernandez
#pragma once

#include <Windows.h>
#include <string>
#include "../GL/glew.h"
#include "../Graphics/System/PSystem.h"

//Cosas a mirar
// 1.-La window existe? -->mirar ok
// 2.-WindowManager destructor. ok




//
namespace Window
{
	class Window;
	class WindowManager : public Singleton<WindowManager>
	{
	public:
		WindowManager() {}
		~WindowManager();
		void Initialize(const char *windowTitle, GLfloat windowWidth, GLfloat windowHeight, GLint show, HINSTANCE hinstance);

		void Update();
		void createWindow();
		Window* GetWindow() { return winInUsage; }

		//Window-Manager Initialize
		void WindowManagerInit();
		//Window-Manager Update
		void WindowManagerUpdate();
		//UnResgisterClass
		void UnregisterWindow();

		//Settors
		void SetWidthHeight(GLfloat width, GLfloat height)
		{
			wWidth = width;
			wHeight = height;
		}

		//Gettors
		HWND getHandler();
		GLfloat getWidht();
		GLfloat getHeight();
		bool WindowExists() const;

	private:
		//        WINDOW INFO               //
		Window * winInUsage;
		HWND wWindowHandle;
		/*HWND wParentHandle;*/
		HINSTANCE  wInstance;

		const char * wTitle;
		GLfloat wWidth, wHeight;

		//state
		bool wExists;



		//       WINDOWMANAGER INFO       //
		std::string wmWindowTitle;
		//Window* wmWindow;
		HINSTANCE wmhinstance;
		WNDCLASSEX	wmWindowClass;
		static bool gWindowExists;
		static HINSTANCE wmAppInstance;

		//forward declaration of the message handler function
		static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	};
}
