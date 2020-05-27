#include "Renderable.h"

#include "../../Transform/Transform2D.h"
#include "../GraphicsSystem.h"
#include "../../GameObject/Component.h"
#include "../../GameObject/GameObject.h"
#include "../Texture.h"
#include "EngineDebug/OpenGL_error.h"
#include "Platform\Resource Manager\ResourceManager.h"

#include <iostream>


namespace Graphics
{
	REFLECTION(Renderable, mTextureName, mLayer, mIsVisible, originalColor);
    Renderable::Renderable() :
        pShader(nullptr)
        , pMesh(nullptr)
        , pTexture(nullptr)
        , mIsVisible(true)
        , pTransform(nullptr)
        , mLayer(0)
		, mTextureName("Default.png")
    {
        m_name = "Renderable";
		mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		originalColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pMesh = (GraphicsSystem::Instance()->pMesh);
		pShader = GraphicsSystem::Instance()->mPrograms[0];
		GraphicsSystem::Instance()->mSpriteLayer[0]->AddRenderable(this);

    }

    Renderable::~Renderable()
    {

    }

    void Renderable::Initialize()
    {
        if (GetOwner())
            pTransform = GetOwner()->m_transform;

        //initialize all the renderables with a default texture.
        if (!pTexture)
            pTexture = ResourceManager::textures[mTextureName].get();

		mColor = originalColor;
		updateInEditMode = true;
		updateInEditPauseMenu = true;
        //GraphicsSystem::Instance()->AddRenderable(this);
    }

    void Renderable::Shutdown()
    {
        //GraphicsSystem::Instance()->RemoveRenderable(this);
    }

    void Renderable::Render(Camera * cameraToUse)
    {
        if (mIsVisible == true)
        {
            if (pTexture != nullptr)
            {
                pTexture->Bind();
            }

			if (pShader != nullptr)
			{
				if (m_GameObject->mode != GameObjectMode::HUD)
				{
					//calculate the matrices that we need
					glm::mat4 view = cameraToUse->GetViewMatrix();
					glm::mat4 projection = cameraToUse->GetProjectionMatrix();
					glm::mat4 modelmtx = m_GameObject->m_transform->getTransformToParent();

					glm::mat4 MVP = projection * view * modelmtx;

					//if (!IsVisible())
					//	return;
					pMesh->Bind();

					//bind the shader we want to use
					pShader->Bind();

					//send the matrices to the shader
					pShader->setUniform("mvp", MVP);
					pShader->setUniform("Color", getColor());
				}
				if(m_GameObject->mode == GameObjectMode::HUD)
				{
					//glm::mat4 view = cameraToUse->GetViewMatrix();
					glm::mat4 projection = cameraToUse->GetProjectionMatrix();
					glm::mat4 modelmtx = pTransform->getTransformToParent();
					glm::mat4 MVP = projection * modelmtx;

					pMesh->Bind();

					//bind the shader we want to use
					pShader->Bind();

					//send the matrices to the shader
					pShader->setUniform("mvp", MVP);
					pShader->setUniform("Color", getColor());
				}
			}
        }

		//GameObject * OBJ = GetOwner();
		//
		//std::cout << "Rendered: " << OBJ->m_name << std::endl;
        //draw the mesh.
        pMesh->draw();
    }
	void Renderable::Flip()
	{
		pTransform->SetScale(glm::vec2(-pTransform->GetScale().x, pTransform->GetScale().y));
	}
    bool Renderable::operator<(const Renderable & str) const
    {
        return (pTransform->mZorder < str.pTransform->mZorder);
    }

	void Renderable::setScaleOfTransform(GLfloat, GLfloat)
	{
		if (pTexture && GetOwner())
			GetOwner()->m_transform->SetScale(glm::vec2(pTexture->GetWidth(), pTexture->GetHeight()));
	}
}