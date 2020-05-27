#include "LineRenderer.h"
#include "../Mesh.h"
#include "../Shader.h"
#include "Engine\Transform\Transform2D.h"
#include "Engine\Graphics\GraphicsSystem.h"
#include "Engine\EngineDebug\OpenGL_error.h"
#include <stdlib.h>  

namespace Graphics
{
	const unsigned int MAX_LINE_COUNT = 512;

	LineRenderer::LineRenderer(GLuint maxLines) : Renderable()
		, mMaxLines(maxLines)
		, mVtxCount(0)
	{
		m_name = "Line Renderer";

		pShader = GraphicsSystem::Instance()->mPrograms[2];

		pMesh = new Mesh(Mesh::EPrimitiveType::eLine);
		
		pMesh->Clear_Vectors();

		// set maximum line count
		pMesh->mPositionList.resize(MAX_LINE_COUNT * 2);
		pMesh->mIndexes.resize(MAX_LINE_COUNT * 2);
		for (std::size_t i = 0; i < pMesh->mIndexes.size(); ++i) // initialize index with dflt values
			pMesh->mIndexes[i] = static_cast<unsigned>(i);

		pMesh->Init();
	}

	LineRenderer::~LineRenderer()
	{
		mVtxCount = 0;
	}

	void LineRenderer::Initialize()
	{
		Renderable::Initialize();
	}

	void LineRenderer::Shutdown()
	{
		Renderable::Shutdown();
	}

	void LineRenderer::Render(Camera * cameraToUse)
	{
		if (!playMode)
		{
			if (mIsVisible)
			{
				//Renderable::Render(cameraToUse);
				if (pShader)
				{
					glm::mat4 view = cameraToUse->GetViewMatrix();
					glm::mat4 projection = cameraToUse->GetProjectionMatrix();
					glm::mat4 VP = projection * view;

					pShader->Bind();

					pShader->setUniform("mvp", VP);
					pShader->setUniform("Color", getColor());
				}

				// bind the mesh
				pMesh->Bind();

				// Pass index data
				//pMesh.UploadToGPU()
				{
					// call gl_buffer 
					glBindBuffer(GL_ARRAY_BUFFER, pMesh->mBufferObjects[0]);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * mVtxCount, pMesh->mPositionList.data());


					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->mBufferObjects[2]);
					glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * mVtxCount, pMesh->mIndexes.data());
				}

				// Draw
				pMesh->draw(mVtxCount);
				gl_error();
			}
		}
		// Flush
		//this->Flush();
	}

	void LineRenderer::DrawLine(glm::vec2 begin, glm::vec2 end)
	{
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(begin);
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(end);
	}

	void LineRenderer::DrawRect(glm::vec2 p0, glm::vec2 p1)
	{
		glm::vec2 p2 = glm::vec2(p0.x + (p1.x - p0.x), p0.y);
		glm::vec2 p3 = glm::vec2(p1.x - (p1.x - p0.x), p1.y);

		//first
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p0);
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p2);

		//second
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p2);
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p1);

		//third
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p1);
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p3);

		//fourth
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p3);
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p0);
	}
	void LineRenderer::DrawRect(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
	{
		//first
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p0);
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p1);

		//second
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p1);
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p3);

		//third
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p3);
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p2);

		//fourth
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p2);
		pMesh->GetPosition()[mVtxCount++] = glm::vec2(p0);
	}

	void LineRenderer::Flush()
	{
		mVtxCount = 0;
	}
}