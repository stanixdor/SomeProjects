#include "Viewport.h"
#include "../Window/WindowManager.h"
#include "Graphics\GraphicsSystem.h"

namespace Graphics
{
	Viewport::Viewport() :mPosition(0.0f, 0.0f)
	{
		Window::WindowManager * wm = Window::WindowManager::Instance();
		mWidth = wm->getWidht();
		mHeight = wm->getHeight();
	}

	Viewport::Viewport(glm::vec2 position, GLfloat width, GLfloat height) : mPosition(position), mWidth(width), mHeight(height)
	{

	}

	void Viewport::Update()
	{
		mWidth = Window::WindowManager::Instance()->getWidht();
		mHeight = Window::WindowManager::Instance()->getHeight();
	}

	void Viewport::Set()
	{
		//We have to enable the scissor tets.
		glEnable(GL_SCISSOR_TEST);

		//in order to do not go out of the viewport
		glScissor((GLint)mPosition.x, (GLint)mPosition.y, (GLsizei)mWidth, (GLsizei)mHeight);
		//matrices.
		//glViewport(mPosition.x, mPosition.y, mWidth, mHeight);
	}

	GLfloat Viewport::GetWidth()
	{
		return mWidth;
	}

	GLfloat Viewport::GetHeight()
	{
		return mHeight;
	}

	GLfloat Viewport::GetAspRatio()
	{
		return GetWidth() / GetHeight();
	}
}