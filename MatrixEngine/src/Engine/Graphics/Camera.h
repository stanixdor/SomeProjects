//Mikel Fernandez
#pragma once

#include "../GLM/glm.hpp"
#include "../GL/glew.h"
#include "Viewport.h"
#include "../GameObject/Component.h"

class Transform2D;

namespace Graphics
{
	class Camera : public Component
	{
	public:
		SERIALIZABLE_NO_PROPERTIES;
		Camera(bool clear = false);
		Camera(glm::vec2 position, GLfloat angle, glm::vec2 scale);
		~Camera();

		void Initialize();
		void Update();
		void Shutdown();

		void zoom(GLfloat zoom);

		glm::mat4 GetViewMatrix();
		//glm::mat4 GetInverseViewMatrix();
		//Our projection matrix is going to be orthogonal I guess.
		glm::mat4 GetProjectionMatrix();
		glm::vec2 GetScale() { return mScale; }
		void SetScaleXY()
		{
			mScale.x = mViewport.GetWidth();
			mScale.y = mViewport.GetHeight();
		}
		void CamSetScale(glm::vec2 newScale)
		{
			mScale.x = newScale.x;
			mScale.y = newScale.y;
		}


		void setPlayerTrans(Transform2D* player);

		glm::vec2 CameraPos();
		bool Clear{ false };
		bool mMouseWindow = false;
		Viewport mViewport;
		Transform2D *mPlayer;
		bool big_room = true;

	private:
		GLfloat mFOVy;
		glm::vec2 mScale;
	};
}
