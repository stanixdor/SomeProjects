//Mikel Fernandez
#pragma once

#pragma once
#include <vector>
#include "glew.h"
#include "../GLM/glm.hpp"

#include <iostream>

namespace Graphics
{
	class Mesh
	{
		friend class LineRenderer;
		friend class FontRenderer;
	public:
		enum EPrimitiveType
		{
			eLine,
			eTriangle
		};

		Mesh();
		Mesh(EPrimitiveType type);
		Mesh(const Mesh &rhs);
		~Mesh();

		//Build quad joining two triangles with the center in vec(0,0).
		void Quad();
		//Clearing the vectors of positions and texture coordinates.
		void Clear_Vectors();
		//Initialize buffers and bind
		void Init(bool dym = false);

		void Bind();

		void draw(GLuint vtxCount = -1);

		void setType(EPrimitiveType type) { mType = type; }

		std::vector<glm::vec2> &GetPosition()
		{
			return mPositionList;
		};

		std::vector<glm::vec2> &GetTexCoord()
		{
			return mTexCoordList;
		};

		Mesh & operator=(const Mesh & rhs);

	private:
		GLuint mVao;
		GLuint mBufferObjects[3];

		EPrimitiveType mType;
		std::vector<glm::vec2> mPositionList;
		std::vector<glm::vec2> mTexCoordList;
		std::vector<GLuint> mIndexes;
	};

}