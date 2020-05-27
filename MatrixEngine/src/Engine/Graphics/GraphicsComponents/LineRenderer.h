#pragma once
#include "Renderable.h"

namespace Graphics
{
	class LineRenderer : public Renderable
	{
	public:
		SERIALIZABLE_NO_PROPERTIES;

		LineRenderer(GLuint maxLines = 2048);
		virtual ~LineRenderer(); 

		virtual void Initialize() override;
		virtual void Shutdown() override;
		virtual void Render(Camera * cameraToUse) override;

		void DrawLine(glm::vec2 begin, glm::vec2 end);
		void DrawRect(glm::vec2 p0, glm::vec2 p1);
		void DrawRect(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);

		void Flush();
	private:
		GLuint mVtxCount;
		GLuint mMaxLines;
	};
}