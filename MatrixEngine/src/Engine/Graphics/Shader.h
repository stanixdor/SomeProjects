//Mikel Fernandez

//Shader class based in Thomas' sample engine
#pragma once


#include "../GL/glew.h"
#include "../GLM/glm.hpp"

#include <string>

namespace Graphics
{

	class Shader
	{
	public: enum eType{eVertex = 0, ePixel/*, eGeometry = GL_GEOMETRY_SHADER*/, eTypeCount};
	public:
		Shader();
		Shader(const char * filename, eType shaderType);

		void Load_Compile(const char * filename = NULL);
		//Load the source
		void LoadSource(const char * filename = NULL);
		//Compile it.
		void Compile();
		//Gettor for the shader type.
		eType GetShaderType() 
		{
			return mShaderType;
		}
		// this will recreate the opengl shader if necessary
		void SetShaderType(eType shaderType, bool createDeviceShader = false);
		void CreateDeviceShader();

	private:
		friend class ShaderProgram;

		GLuint mGLHandler;
		std::string	mSource;
		std::string mFilename;
		
		eType mShaderType;
	};


	class ShaderProgram
	{
	public:
		ShaderProgram();
		ShaderProgram(Shader * vertexShader, Shader * fragmentShader);
		//ShaderProgram(Shader * vertexShader, Shader * fragmentShader, Shader * geometryShader);
		~ShaderProgram();

		void Bind();
		void Unbind();
		void Load();

		Shader* GetShader(Shader::eType shaderType);
		void SetShader(Shader * pShader, bool link = false);

		GLuint GetOpenGLHandle();

		//Uniforms
		void   setUniform(const char *name, const glm::vec2 & v) const;
		void   setUniform(const char *name, const glm::vec3 & v) const;
		void   setUniform(const char *name, const glm::vec4 & v) const;
		void   setUniform(const char *name, const glm::mat4 & m) const;
		void   setUniform(const char *name, const glm::mat3 & m) const;
		void   setUniform(const char *name, GLfloat val) const;
		void   setUniform(const char *name, GLint val) const;
		void   setUniform(const char *name, GLuint val) const;
		void   setUniform(const char *name, bool val) const;

	private:
		friend class Shader;
		void Link();
		void AttachShader(Shader::eType which = Shader::eTypeCount);
		void DetachShader(Shader::eType which = Shader::eTypeCount);

		GLuint mGLHandler;
		Shader * mShaders[Shader::eTypeCount];
	};
}