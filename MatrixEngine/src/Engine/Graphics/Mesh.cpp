//Mikel Fernandez


#include "Mesh.h"

#include "../GL/glew.h"

namespace Graphics
{
	Mesh::Mesh(EPrimitiveType type) : mType(type)
	{
		//generate vertex array object names.
		glGenVertexArrays(1, &mVao);
		//generate buffer object names.
		glGenBuffers(3, mBufferObjects);
	}

	Mesh::Mesh() : mType(eTriangle)
	{
		glGenVertexArrays(1, &mVao);
		glGenBuffers(3, mBufferObjects);
		Quad();
		Init();
	}
	Mesh::Mesh(const Mesh &rhs)
	{
		glGenVertexArrays(1, &mVao);
		glGenBuffers(3, mBufferObjects);

		mType = rhs.mType;
		mPositionList = rhs.mPositionList;
		mTexCoordList = rhs.mTexCoordList;
		mIndexes = rhs.mIndexes;
		Init();
	}
	Mesh &Mesh::operator=(const Mesh &rhs)
	{
		glGenVertexArrays(1, &mVao);
		glGenBuffers(3, mBufferObjects);

		mType = rhs.mType;
		mPositionList = rhs.mPositionList;
		mTexCoordList = rhs.mTexCoordList;
		mIndexes = rhs.mIndexes;
		Init();

		return *this;
	}
	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &mVao);
		glDeleteBuffers(3, mBufferObjects);
		Clear_Vectors();
	}

	void Mesh::Quad()
	{
		//vertices
		mPositionList.push_back(glm::vec2(-0.5f, 0.5f));
		mPositionList.push_back(glm::vec2(-0.5f, -0.5f));
		mPositionList.push_back(glm::vec2(0.5f, -0.5f));
		mPositionList.push_back(glm::vec2(0.5f, 0.5f));

		//texture coordinates
		mTexCoordList.push_back(glm::vec2(0.0f, 1.0f));
		mTexCoordList.push_back(glm::vec2(0.0f, 0.0f));
		mTexCoordList.push_back(glm::vec2(1.0f, 0.0f));
		mTexCoordList.push_back(glm::vec2(1.0f, 1.0f));

		//indexes
		mIndexes.push_back(0);
		mIndexes.push_back(1);
		mIndexes.push_back(2);
		mIndexes.push_back(0);
		mIndexes.push_back(2);
		mIndexes.push_back(3);
	}

	void Mesh::Clear_Vectors()
	{
		mPositionList.clear();
		mTexCoordList.clear();
		mIndexes.clear();
	}

	void Mesh::Init(bool dym)
	{
		//bind out vertex array object.
		glBindVertexArray(mVao);


		//                MPOSITIONS                 
		//bind mpositions buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, mBufferObjects[0]);
		//creates and initializes mpositions's data store.
		glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(mPositionList.size() * sizeof(GLfloat) * 2.0f), mPositionList.data(), dym ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		//Enable mposition
		glEnableVertexAttribArray(0);
		//define an array of mpositions vertex attribute data.
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


		//               MTEXCOORDS
		//bind mtexcoords buffer object.
		glBindBuffer(GL_ARRAY_BUFFER, mBufferObjects[1]);
		//creates and initializes mtexcoords's data store.
		glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(mTexCoordList.size() * sizeof(GLfloat) * 2.0f), mTexCoordList.data(), dym ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		//Enable mtexcoords
		glEnableVertexAttribArray(1);
		//define an array of mtexcoords vertex attribute data.
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//               MINDEXES
		//bind mtexcoords buffer object.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferObjects[2]);
		//creates and initializes mtexcoords's data store.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexes.size() * sizeof(GLuint), mIndexes.data(), dym ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	}
	void Mesh::Bind()
	{
		glBindVertexArray(mVao);
	}
	void Mesh::draw(GLuint vtxCount)
	{
		glBindVertexArray(mVao);

		if (vtxCount == -1)
			vtxCount = static_cast<GLuint>(mPositionList.size());
		//draw taking into account the type of shape we want to draw.
		if (mType == eTriangle)
			glDrawElements(GL_TRIANGLES, GLsizei(mIndexes.size()), GL_UNSIGNED_INT, 0);
		else if (mType == eLine)
			glDrawElements(GL_LINES, vtxCount, GL_UNSIGNED_INT, 0);

		//unbind
		glBindVertexArray(0);
	}
}