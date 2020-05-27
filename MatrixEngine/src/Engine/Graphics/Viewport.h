// Mikel Fernandez
#pragma once

#include "../GLM/glm.hpp"
#include "../GL/glew.h"

namespace Graphics
{
	class Viewport
	{
	public:
		Viewport();
		Viewport(glm::vec2 position, GLfloat width, GLfloat height);
		Viewport(const Viewport&) = default;

		void Update();

		void Set();

		//glm::mat3 computeViewportMtx();
		GLfloat GetWidth();
		GLfloat GetHeight();
		GLfloat GetAspRatio();
		glm::vec2 GetPos(){return mPosition;}

		GLfloat mWidth = 800;
		GLfloat mHeight = 600;
	private:
		glm::vec2 mPosition;
		GLfloat aspectRatio;
	};
}