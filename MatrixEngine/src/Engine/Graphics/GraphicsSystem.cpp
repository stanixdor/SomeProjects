//Mikel Fernandez


#include "GraphicsSystem.h"
#include "../Window/WindowManager.h"

//#include "Winuser.h"
#include "../GameObject/GameObject.h"
#include "Texture.h"
#include "GraphicsComponents\LineRenderer.h"
#include "GraphicsComponents\AnimationComponent.h"
#include "SpriteAnimationData.h"
#include "../ImGui/imgui.h"
#include "../../src/Engine/Editor/ImGuiFunc.h"
#include "Editor\ImGuiUpdate.h"
#include "Engine\Platform\Resource Manager\ResourceManager.h"
#include "Platform\Input\Input.h"

#ifndef GLEW_STATIC

#define GLEW_STATIC

#endif // !GLEW_STATIC

#include <glew.h>
#include "wglew.h"
#include <memory>
#include <iostream>
#include <algorithm>

bool playMode = true;
bool PAUSE = false;

void GraphicsSystem::Initialize(HWND wWindowHandle)
{
	InitializeOpenGL(wWindowHandle);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initializeLayers();
	LoadShaders();
	LoadTextures();
}

void GraphicsSystem::initializeLayers()
{
	mSpriteLayer.push_back(new Graphics::SpriteLayer("PLAYABLE", Graphics::SpriteLayer::LayerIndex::PLAYABLE));
	//mSpriteLayer.push_back(new Graphics::SpriteLayer("IMGUI", Graphics::SpriteLayer::LayerIndex::IMGUI));
}

void GraphicsSystem::UpdateCameras()
{
	for (const auto & it : mSpriteLayer)
	{
		//SECURITY CHECK
		if(it != nullptr && it->mCameraLayer != nullptr)
			it->mCameraLayer->Update();
	}
}

void GraphicsSystem::endOfFrameUpdate()
{
	for (auto& layer : mSpriteLayer)
	{
		layer->endOfFrameUpdate();
	}
}

void GraphicsSystem::UpdateCamerasZoom(GLfloat theZoom)
{
	for (const auto & it : mSpriteLayer)
	{
		if(it->mCameraLayer != nullptr)
			it->mCameraLayer->zoom(theZoom);
	}
}

void GraphicsSystem::Update()
{
	Window::WindowManager::Instance()->Update();
	//UpdateCameras();
	render();
}

void GraphicsSystem::render()
{
	/*Graphics::LineRenderer * line = new Graphics::LineRenderer();
	line->DrawLine(glm::vec2(0, 0), glm::vec2(1, 1));*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glViewport(0, 0, GLsizei(Window::WindowManager::Instance()->getWidht()), GLsizei(Window::WindowManager::Instance()->getHeight()));
	for (auto & it : mSpriteLayer)
	{
		it->render();
	}

	//glViewport(0, 0, GLsizei(Window::WindowManager::Instance()->getWidht()/4), GLsizei(Window::WindowManager::Instance()->getHeight()/4));
	//for (auto & it : mSpriteLayer)
	//{
	//	it->render();
	//}

	ImGui_NewFrame();

	//** FUNCTIONS FOR UPDATE IMGUI GO HERE  **//

	EngineImGUI::Update();

	glViewport(0, 0, GLsizei(Window::WindowManager::Instance()->getWidht()), GLsizei(Window::WindowManager::Instance()->getHeight()));
	ImGui::Render();

	SwapBuffers(gDeviceContext);
}

void GraphicsSystem::LoadShaders()
{
	Graphics::Shader * vertex = new Graphics::Shader("./Shader/basic.vs", Graphics::Shader::eVertex);
	Graphics::Shader * fragment = new Graphics::Shader("./Shader/basic.fs", Graphics::Shader::ePixel);
	Graphics::Shader * vertex1 = new Graphics::Shader("./Shader/Animations.vs", Graphics::Shader::eVertex);
	Graphics::Shader * fragment1 = new Graphics::Shader("./Shader/Animations.fs", Graphics::Shader::ePixel);
	Graphics::Shader * vertex2 = new Graphics::Shader("./Shader/Lines.vs", Graphics::Shader::eVertex);
	Graphics::Shader * fragment2 = new Graphics::Shader("./Shader/Lines.fs", Graphics::Shader::ePixel);
	Graphics::Shader * vertex3 = new Graphics::Shader("./Shader/Text.vs", Graphics::Shader::eVertex);
	Graphics::Shader * fragment3 = new Graphics::Shader("./Shader/Text.fs", Graphics::Shader::ePixel);

	mPrograms.push_back(new ShaderProgram(vertex, fragment));
	mPrograms.push_back(new ShaderProgram(vertex1, fragment1));
	mPrograms.push_back(new ShaderProgram(vertex2, fragment2));
	mPrograms.push_back(new ShaderProgram(vertex3, fragment3));
}

void GraphicsSystem::LoadTextures()
{
	pMesh = new Mesh();
	IdleAnim_data3 = new SpriteAnimationData("Idle", new Texture("Data/Textures/Default.png"), 1, 1, 2.0f);
	/*
	mTextures.push_back(new Texture("Images/agachar.png"));
	mTextures.push_back(new Texture("Images/TestDibo.png"));
	mTextures.push_back(new Texture("Images/AnimationTest.png"));
	mTextures.push_back(new Texture("Images/gacela.png"));
	mTextures.push_back(new Texture("Images/greenbackground.png"));
	mTextures.push_back(new Texture("Images/redbackground.png"));
	mTextures.push_back(new Texture("Images/Select.png"));
	mTextures.push_back(new Texture("Images/Translate.png"));
	mTextures.push_back(new Texture("Images/Scale.png"));
	mTextures.push_back(new Texture("Images/Rotate.png"));*/
}

void GraphicsSystem::AddCamera(Camera* camera)
{
	for (auto it = (mCameras).begin(); it != (mCameras).end(); ++it)
		if ((*it) == camera)
			return;

	mCameras.push_back(camera);
}

void GraphicsSystem::RemoveCamera(Camera* camera)
{
	for (auto it = (mCameras).begin(); it != (mCameras).end(); ++it)
	{
		if ((*it) == camera)
		{
			mCameras.erase(it);
			return;
		}
	}
}

void GraphicsSystem::AddRenderable(Renderable * pRenderable)
{
	for (const auto & it : mSpriteLayer)
		if (it->mLayerIndex == pRenderable->mLayer)
			it->AddRenderable(pRenderable);
}

void GraphicsSystem::RemoveRenderable(Renderable * pRenderable)
{
	for (const auto & it : mSpriteLayer)
		if (it->mLayerIndex == pRenderable->mLayer)
			it->RemoveRenderable(pRenderable);
}

void GraphicsSystem::clearLayers()
{
	for (auto& SpriteLayer : mSpriteLayer)
	{
		SpriteLayer->clearLayer();
	}
}

void GraphicsSystem::InitializeOpenGL(HWND wWindowHandle)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	//Get the device context
	gDeviceContext = GetDC(wWindowHandle);
	if (gDeviceContext == nullptr)
		std::cout << "Failed to get the device context" << std::endl;

	//Choose pixel format.
	int choose_pixel_format = ChoosePixelFormat(gDeviceContext, &pfd);
	if (choose_pixel_format == 0)
		std::cout << "Failed to choose pixel format" << std::endl;

	//Set the pixel format
	if (SetPixelFormat(gDeviceContext, choose_pixel_format, &pfd) == false)
		std::cout << "Failed to set the pixel format" << std::endl;

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};

	//Create the fake context
	gRenderContext = wglCreateContext(gDeviceContext);
	//make it current
	wglMakeCurrent(gDeviceContext, gRenderContext);
	if (gRenderContext == nullptr)
		std::cout << "Failed to create or make it current" << std::endl;


	GLenum er = glewInit();
	if (er != GLEW_OK)
		std::cout << "Failed to Initialize glew" << std::endl;

	if (glewIsSupported("WGL_ARB_create_context") == 1)
	{
		HGLRC temp = gRenderContext;
		gRenderContext = wglCreateContextAttribsARB(gDeviceContext, 0, attribs);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(temp);
		wglMakeCurrent(gDeviceContext, gRenderContext);
	}
}
