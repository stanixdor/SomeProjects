//Mikel Fernandez
#pragma once

#include "System\PSystem.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Viewport.h"
#include "GraphicsComponents/Renderable.h"
#include "Engine\Graphics\SpriteAnimationData.h"

#include "SpriteLayer.h"

//#include <Windows.h>

#include <vector>

using namespace Graphics;

class GraphicsSystem : public Singleton<GraphicsSystem>
{
public:
	void Initialize(HWND wWindowHandle);
	void initializeLayers();


	//this will call to render.
	void Update();

	//render 
	void render();

	void LoadShaders();
	void LoadTextures();

	//just in case we have more than one camera.
	void AddCamera(Camera* camera);
	void RemoveCamera(Camera* camera);

	void AddRenderable(Renderable * pRenderable);
	void RemoveRenderable(Renderable * pRenderable);
	void clearLayers();

	/*void setShaderProgram(ShaderProgram* prog);
	ShaderProgram* getShaderProgram();*/
	void UpdateCamerasZoom(GLfloat theZoom);
	void UpdateCameras();

	void endOfFrameUpdate();

	std::vector<SpriteLayer *> mSpriteLayer;
	std::vector<ShaderProgram*> mPrograms;
	//std::vector<Texture*> mTextures;
	std::vector<Camera*> mCameras;
	Mesh* pMesh;
	SpriteAnimationData * IdleAnim_data3;

private:
	void InitializeOpenGL(HWND wWindowHandle);

	HDC gDeviceContext = nullptr;
	HGLRC gRenderContext = nullptr;

	std::vector<Renderable*> mRenderables;
};

extern bool playMode;
extern bool PAUSE;

