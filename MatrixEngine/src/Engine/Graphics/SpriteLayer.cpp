#include "SpriteLayer.h"

#include "Shader.h"
#include "Transform\Transform2D.h"
#include "GameObject\GameObject.h"
#include <iostream>
#include <algorithm>

namespace Graphics
{
	SpriteLayer::SpriteLayer(std::string name, LayerIndex layer, Camera * mCamera) : mCameraLayer(mCamera)
																, mName(name)
																, mLayerIndex(layer)
																, mRenderables(NULL)
																, mIsVisible(true)
	{
	}

	SpriteLayer::~SpriteLayer()
	{

	}

	void SpriteLayer::setOrder(GLuint pos)
	{
		mLayerIndex = (LayerIndex)pos; 
	}

	void SpriteLayer::AddRenderable(Renderable * pRend)
	{
		/*for (const auto & it : mRenderables)
			if (it == pRend)
				return;*/

		mRenderables.push_back(pRend);
		pRend->SetLayer(mLayerIndex);
	}

	void SpriteLayer::RemoveRenderable(Renderable * pRend)
	{
		for (auto it = mRenderables.begin(); it != mRenderables.end(); it++)
		{
			if ((*it) == pRend)
			{
				mRenderables.erase(it);
				return;
			}
		}
		pRend->SetLayer(0);
	}

	void SpriteLayer::clearLayer()
	{
		mRenderables.clear();
	}

	void SpriteLayer::render()
	{
		//if (mCameraLayer->Clear)
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	mCameraLayer->mViewport.Set();
		if (mCameraLayer == nullptr)
			return;
		orderRenderables();
		for (const auto & renderables : mRenderables)
		{
			renderables->Render(mCameraLayer);
			if (renderables == nullptr)
				RemoveRenderable(renderables);
		}
	}

	void SpriteLayer::orderRenderables()
	{
		std::stable_sort(mRenderables.begin(), mRenderables.end(), [](const Renderable* a, const Renderable* b)
		{
			return a->m_GameObject->m_transform->mZorder < b->m_GameObject->m_transform->mZorder;
		});
	}

	void SpriteLayer::endOfFrameUpdate()
	{
		auto it = mRenderables.begin();
		while ( it != mRenderables.end())
		{
			if ((*it)->m_alive == false)
			{
				it = mRenderables.erase(it);
			}
			else
				it++;

		}
	}

}