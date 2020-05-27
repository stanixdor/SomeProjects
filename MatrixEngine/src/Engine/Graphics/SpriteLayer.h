//Mikel Fernandez

//we will need this for the different render layers in our game.
#pragma once

#include "Camera.h"
#include "GraphicsComponents/Renderable.h"

#include <vector>

namespace Graphics
{
	class SpriteLayer
	{
		//friend class GraphicsSystem;
	public:
		enum LayerIndex {
			STATICBG = 0,
			BG = 1,
			PLAYABLE = 2,
			HUD = 3,
			IMGUI = 4
		};

		SpriteLayer(std::string name, LayerIndex layer, Camera * mCamera = nullptr);
		~SpriteLayer();

		void setOrder(GLuint pos);

		void AddRenderable(Renderable * pRend);
		void RemoveRenderable(Renderable * pRend);
		void clearLayer();
		void render();

		void setCamera(Camera *camera) { mCameraLayer = camera; }

		//ordenar objetos
		void orderRenderables();

	
		Camera *mCameraLayer;
		std::string mName;
		LayerIndex mLayerIndex;
		std::vector<Renderable*> mRenderables;
		bool mIsVisible;
		void endOfFrameUpdate();
	};
}
