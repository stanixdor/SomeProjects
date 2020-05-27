#include "../Window/WindowManager.h"
#include "Camera.h"
#include "../Graphics/GraphicsSystem.h"
#include "../Transform/Transform2D.h"
#include "../GLM/gtc/matrix_transform.hpp"
#include "../GLM/gtx/matrix_transform_2d.hpp"
#include "../Window/Window.h"
#include "../../Engine/GameObject/GameObject.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

namespace Graphics
{
	
	Camera::Camera(bool clear) : Component()
		, mViewport()
		, Clear(clear)
		, mPlayer(nullptr)
	{
		m_name = "Camera";
		mScale.x = 2560;
		mScale.y = 1440;
		mFOVy = static_cast<GLfloat>(M_PI / 4);
	}

	Camera::Camera(glm::vec2 position, GLfloat angle, glm::vec2 scale) : Component()
		, Clear(true)
		, mViewport(position, scale.x, scale.y)
	{
		m_name = "Camera";
		mFOVy = static_cast<GLfloat>(angle / 4);
	}

	Camera::~Camera()
	{
		this->Shutdown();
	}

	void Camera::Initialize()
	{
		GraphicsSystem::Instance()->AddCamera(this);
	}

	void Camera::Update()
	{
		mViewport.Update();
		mViewport.Set();
	}

	void Camera::Shutdown()
	{
		GraphicsSystem::Instance()->RemoveCamera(this);
	}

	void Camera::zoom(GLfloat zoom)
	{
		mScale *= zoom;
	}

	void Camera::setPlayerTrans(Transform2D* player)
	{
		if(player != nullptr)
			mPlayer = player;
	}

	glm::vec2 Camera::CameraPos()
	{
		if (mPlayer != nullptr)
			return mPlayer->GetPos();

		else
			return m_GameObject->m_transform->GetPos();
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		glm::vec3 eye;
		glm::vec3 center;

		if (mPlayer && big_room)
		{
			eye = glm::vec3(mPlayer->GetPos(), 1.0f);
			center = glm::vec3(mPlayer->GetPos(), 0.0f);
		}
		else
		{
			eye = glm::vec3(m_GameObject->m_transform->GetPos(), 1.0f);
			center = glm::vec3(m_GameObject->m_transform->GetPos(), 0.0f);
		}

		glm::mat4 result = glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));

		return result;
	}

	/*glm::mat4 Camera::GetInverseViewMatrix()
	{
		glm::mat4 InvView, transMTX, scaMTX, rotMTX;

		transMTX = glm::translate(glm::mat4(1.0f), glm::vec3((mTransform->GetPos(), 0.0f)));
		scaMTX = glm::scale(glm::mat4(1.0f), glm::vec3(mTransform->GetScale().x, mTransform->GetScale().y, 0.0f));
		rotMTX = glm::rotate(glm::mat4(1.0f), (mTransform->GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f));

		InvView = transMTX * rotMTX * scaMTX;

		return InvView;
	}*/

	glm::mat4 Camera::GetProjectionMatrix()
	{
		//return glm::perspective(mFOVy, mViewport.GetWidth() / mViewport.GetHeight(), mNearPlane, mFarPlane);
		//return glm::ortho(-mViewport.GetWidth() / 2.0f, mViewport.GetWidth() / 2.0f, -mViewport.GetHeight() / 2.0f, mViewport.GetHeight() / 2.0f);
		return glm::ortho(-mScale.x / 2.0f, mScale.x / 2.0f, -mScale.y / 2.0f, mScale.y / 2.0f);
	}
}