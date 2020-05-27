//Mikel Fernandez


#include "Shader.h"

#include "../GL/wglew.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace Graphics
{
	Shader::Shader() : mShaderType(eTypeCount), mGLHandler(0)
	{

	}

	Shader::Shader(const char * filename, eType shaderType) : mShaderType(shaderType), mGLHandler(0)
	{
		mFilename = filename;

		Load_Compile(filename);
	}

	void Shader::Load_Compile(const char * filename)
	{
		LoadSource(filename);
		Compile();
	}

	void Shader::LoadSource(const char * filename)
	{
		if (filename)
			mFilename = filename;

		//open the shader file and load its content
		std::fstream shaderFile(mFilename.c_str(), std::ios::in);
		std::stringstream shaderSource;
		//rdbuf : Returns a pointer to the internal filebuf object.
		shaderSource << shaderFile.rdbuf();

		//save it 
		mSource = shaderSource.str();
	}

	void Shader::Compile()
	{
		//check if the source is empty. if yes, return.
		if (mSource.empty())
			return;

		//create shader(delete if neccesary)
		bool created = mGLHandler != 0;
		SetShaderType(mShaderType, !created);

		//We pass all the code of our source to OpenGL
		const char * mSourceAux = mSource.c_str();
		glShaderSource(mGLHandler, 1, &mSourceAux, NULL);

		//Compile the code.
		glCompileShader(mGLHandler);

		// sanity check
		GLint result;
		glGetShaderiv(mGLHandler, GL_COMPILE_STATUS, &result);
		if (GL_FALSE == result)
		{
			// error log
			GLint logLen;
			glGetShaderiv(mGLHandler, GL_INFO_LOG_LENGTH, &logLen);
			if (logLen > 0)
			{
				char * log = (char *)malloc(logLen);
				GLsizei written;
				glGetShaderInfoLog(mGLHandler, logLen, &written, log);
				char str[100];
				sprintf_s(str, "Shader Compilation Error: %s", mFilename.c_str());
				MessageBoxA(NULL, log, str, MB_TASKMODAL | MB_SETFOREGROUND | MB_ICONERROR);
				free(log);
			}
		}
	}

	void Shader::SetShaderType(eType shaderType, bool createDeviceShader)
	{
		mShaderType = shaderType;

		if (createDeviceShader)
			CreateDeviceShader();
	}

	void Shader::CreateDeviceShader()
	{
		if (mGLHandler)
			glDeleteShader(mGLHandler);
		// create a new shader
		GLenum type = mShaderType == eVertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		mGLHandler = glCreateShader(type);
	}


	//                  SHADER-PROGRAM
	ShaderProgram::ShaderProgram() : mGLHandler(0)
	{
		// create the shader program
		mGLHandler = glCreateProgram();
		std::memset(mShaders, NULL, sizeof(Shader*)*Shader::eTypeCount);
	}

	ShaderProgram::ShaderProgram(Shader * vertexShader, Shader * fragmentShader)
	{

		// create the shader program
		mGLHandler = glCreateProgram();
		std::memset(mShaders, NULL, sizeof(Shader*)*Shader::eTypeCount);
		// set shader pointers
		if (vertexShader)
			mShaders[vertexShader->mShaderType] = vertexShader;
		if (fragmentShader)
			mShaders[fragmentShader->mShaderType] = fragmentShader;
		// Link with shaders right away
		Link();
	}

	void ShaderProgram::Link()
	{
		for (GLuint i = 0; i < Shader::eTypeCount; ++i) {
			if (mShaders[i]) {
				glAttachShader(mGLHandler, mShaders[i]->mGLHandler);
			}
		}

		// link the shader
		glLinkProgram(mGLHandler);

		// error check
		GLint status;
		glGetProgramiv(mGLHandler, GL_LINK_STATUS, &status);
		if (GL_FALSE == status)
		{
			GLint logLen;
			glGetProgramiv(mGLHandler, GL_INFO_LOG_LENGTH, &logLen);
			if (logLen > 0)
			{
				char * log = (char *)malloc(logLen);
				GLsizei written;
				glGetProgramInfoLog(mGLHandler, logLen, &written, log);
				MessageBoxA(NULL, log, "Shader Linking Error", MB_TASKMODAL | MB_SETFOREGROUND | MB_ICONERROR);
				free(log);
			}
		}

		// detach shaders
		DetachShader();
	}

	ShaderProgram::~ShaderProgram()
	{

	}

	void ShaderProgram::Bind()
	{
		//use the expected program.
		glUseProgram(mGLHandler);
	}

	void ShaderProgram::Unbind()
	{
		glUseProgram(NULL);
	}

	void ShaderProgram::Load()
	{
		for (GLuint i = 0; i < Shader::eTypeCount; ++i) {
			if (mShaders[i]) {
				mShaders[i]->Load_Compile();
			}
		}

		Link();
	}

	Shader* ShaderProgram::GetShader(Shader::eType shaderType)
	{
		if (shaderType < Shader::eTypeCount)
			return mShaders[shaderType];
		return NULL;
	}
	void ShaderProgram::SetShader(Shader * pShader, bool link)
	{
		if (!pShader)
			return;

		Shader::eType & shaderType = pShader->mShaderType;

		// set shader
		DetachShader(shaderType);
		mShaders[shaderType] = pShader;

		// link if necessary
		if (link)
			Link();
	}
	void ShaderProgram::AttachShader(Shader::eType shaderType)//deflt = all
	{
		// specific shadertype ->set if necessary
		if (shaderType != Shader::eTypeCount) {
			if (mShaders[shaderType]) {
				glAttachShader(mGLHandler, mShaders[shaderType]->mGLHandler);
			}
		}
		else
		{
			for (int i = 0; i < Shader::eTypeCount; ++i) {
				if (mShaders[i]) {
					glAttachShader(mGLHandler, mShaders[i]->mGLHandler);
				}
			}
		}
	}
	void ShaderProgram::DetachShader(Shader::eType shaderType)//deflt = all
	{
		// specific shadertype ->set if necessary
		if (shaderType != Shader::eTypeCount) {
			if (mShaders[shaderType]) {
				glDetachShader(mGLHandler, mShaders[shaderType]->mGLHandler);
			}
		}
		else
		{
			for (int i = 0; i < Shader::eTypeCount; ++i) {
				if (mShaders[i]) {
					glDetachShader(mGLHandler, mShaders[i]->mGLHandler);
				}
			}
		}
	}

	GLuint ShaderProgram::GetOpenGLHandle()
	{
		return mGLHandler;
	}

	using namespace std;
	void ShaderProgram::setUniform(const char *name, const glm::vec2 & v) const
	{
		GLint loc = glGetUniformLocation(mGLHandler, name);

		if (loc >= 0)
			glUniform2f(loc, v.x, v.y);
		else
			cout << "Uniform" << name << "not found" << endl;
	}

	void   ShaderProgram::setUniform(const char *name, const glm::vec3 & v) const
	{
		GLint loc = glGetUniformLocation(mGLHandler, name);

		if (loc >= 0)
			glUniform3f(loc, v.x, v.y, v.z);
		else
			cout << "Uniform" << name << "not found" << endl;
	}

	void   ShaderProgram::setUniform(const char *name, const glm::vec4 & v) const
	{
		GLint loc = glGetUniformLocation(mGLHandler, name);

		if (loc >= 0)
			glUniform4f(loc, v.x, v.y, v.z, v.w);
		else
			cout << "Uniform" << name << "not found" << endl;
	}

	void   ShaderProgram::setUniform(const char *name, const glm::mat4 & m) const
	{
		GLint loc = glGetUniformLocation(mGLHandler, name);
		if (loc >= 0)
			glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
		else
			cout << "Uniform" << name << "not found." << endl;
	}

	void   ShaderProgram::setUniform(const char *name, const glm::mat3 & m) const
	{
		GLint loc = glGetUniformLocation(mGLHandler, name);
		if (loc >= 0)
			glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
		else
			cout << "Uniform" << name << "not found." << endl;
	}

	void   ShaderProgram::setUniform(const char *name, GLfloat val) const
	{
		GLint loc = glGetUniformLocation(mGLHandler, name);
		if (loc >= 0)
			glUniform1f(loc, val);
		else
			cout << "Uniform" << name << "not found." << endl;
	}

	void   ShaderProgram::setUniform(const char *name, GLint val) const
	{
		GLint loc = glGetUniformLocation(mGLHandler, name);
		if (loc >= 0)
			glUniform1i(loc, val);
		else
			cout << "Uniform" << name << "not found." << endl;
	}

	void   ShaderProgram::setUniform(const char *name, GLuint val) const
	{
		GLint loc = glGetUniformLocation(mGLHandler, name);
		if (loc >= 0)
			glUniform1ui(loc, val);
		else
			cout << "Uniform" << name << "not found." << endl;
	}

	void   ShaderProgram::setUniform(const char *name, bool val) const
	{
		GLint loc = glGetUniformLocation(mGLHandler, name);
		if (loc >= 0)
			glUniform1i(loc, val);
		else
			cout << "Uniform" << name << "not found." << endl;
	}
}