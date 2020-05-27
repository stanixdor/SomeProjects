//Mikel Fernandez
#pragma once

#include "glew.h"
#include "../../GameObject/Component.h"
#include "../Camera.h"
#include "../Texture.h"
#include "Engine\Graphics\Mesh.h"

class Transform2D;

namespace Graphics
{
	//forward declarations
	class Texture;
	class Shader;
	class ShaderProgram;

	class Renderable : public Component
	{

	public:
		SERIALIZABLE;
		Renderable();
		//Renderable(const Renderable & rend);
		virtual ~Renderable();
		virtual void Initialize();
		virtual void Shutdown();
		virtual void Render(Camera * cameraToUse);

		void Flip();

		void setScaleOfTransform(GLfloat x, GLfloat y);

		bool IsVisible() { return mIsVisible; }
		void SetVisible(bool visible) { mIsVisible = visible; }
		Transform2D * GetTransform() { return pTransform; }

		void SetLayer(GLint whichLayer) { mLayer = whichLayer; }

		bool operator<(const Renderable& str) const;

		glm::vec4 getColor() { return mColor; }
		void setColor(glm::vec4 color) { mColor = color; }
		void setOriginalColor(glm::vec4 color) { originalColor = color; }
		void SetAlpha(float Alpha) { mColor[3] = Alpha; }

		ShaderProgram *pShader;
		Mesh *pMesh;
		Texture	*pTexture;
		std::string mTextureName;

		bool exist = false;

	public:
		//friend class Graphics;
		bool mIsVisible;
		Transform2D	*pTransform;
		GLint mLayer;

		glm::vec4 mColor;
		glm::vec4 originalColor;

	};
}