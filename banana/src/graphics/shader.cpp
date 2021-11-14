#include"banana/graphics/shader.h"
#include"banana/log.h"
#include"banana/graphics/helper.h"
#include"glad/glad.h"
#include"external/glm/gtc/type_ptr.hpp"

namespace banana::graphics
{
	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
		:m_VertexSource(vertexSource)
		,m_FragmentSource(fragmentSource)
	{
		m_ProgramId = glCreateProgram();	BANANA_CHECK_GL_ERROR

		int status = GL_FALSE;
		char errorLog[512];
		uint32_t vertexShaderId = glCreateShader(GL_VERTEX_SHADER); BANANA_CHECK_GL_ERROR

		{
			const GLchar* vertSource = vertexSource.c_str();
			glShaderSource(vertexShaderId, 1, &vertSource, NULL);	BANANA_CHECK_GL_ERROR
			glCompileShader(vertexShaderId);	BANANA_CHECK_GL_ERROR
			glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &status);	BANANA_CHECK_GL_ERROR
			if (status != GL_TRUE)
			{
				glGetShaderInfoLog(vertexShaderId, sizeof(errorLog), NULL, errorLog);	BANANA_CHECK_GL_ERROR
				BANANA_ERROR("Vertex Shader Compilation Error : {}",errorLog);
			}
			else
			{
				glAttachShader(m_ProgramId, vertexShaderId);	BANANA_CHECK_GL_ERROR
			}
		}

		uint32_t fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);			BANANA_CHECK_GL_ERROR
		if(status == GL_TRUE)
		{
			const GLchar* fragSource = fragmentSource.c_str();
			glShaderSource(fragmentShaderId, 1, &fragSource, NULL);		BANANA_CHECK_GL_ERROR
			glCompileShader(fragmentShaderId);		BANANA_CHECK_GL_ERROR
			glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &status);		BANANA_CHECK_GL_ERROR
			if (status != GL_TRUE)
			{
				glGetShaderInfoLog(fragmentShaderId, sizeof(errorLog), NULL, errorLog);		BANANA_CHECK_GL_ERROR
				BANANA_ERROR("Fragment Shader Compilation Error: {}",errorLog);
			}
			else
			{
				glAttachShader(m_ProgramId, fragmentShaderId);		BANANA_CHECK_GL_ERROR
			}
		}

		BANANA_ASSERT(status == GL_TRUE, "Error Compiling Shader");
		if (status == GL_TRUE)
		{
			glLinkProgram(m_ProgramId);		BANANA_CHECK_GL_ERROR
			glValidateProgram(m_ProgramId);		BANANA_CHECK_GL_ERROR
			glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &status);		BANANA_CHECK_GL_ERROR
			if (status != GL_TRUE)
			{
				glGetProgramInfoLog(m_ProgramId, sizeof(errorLog), NULL, errorLog);		BANANA_CHECK_GL_ERROR
				BANANA_ERROR("shader link Error: {}", errorLog);
				glDeleteProgram(m_ProgramId);		BANANA_CHECK_GL_ERROR
				m_ProgramId = -1;//inavlid value
			}

		}

		glDeleteShader(vertexShaderId);		BANANA_CHECK_GL_ERROR
		glDeleteShader(fragmentShaderId);		BANANA_CHECK_GL_ERROR

	}

	Shader::~Shader()
	{
		glUseProgram(0);			// gl error here prolly related to destruction oreder BANANA_CHECK_GL_ERROR
		glDeleteProgram(m_ProgramId);		// BANANA_CHECK_GL_ERROR	
	}


	void Shader::bind()
	{
		glUseProgram(m_ProgramId);		BANANA_CHECK_GL_ERROR
	}

	void Shader::unbind()
	{
		glUseProgram(0);		BANANA_CHECK_GL_ERROR
	}

	void Shader::setUniformInt(const std::string& name, int val)
	{
		bind();
		glUniform1i(getUniformLocation(name),val);		BANANA_CHECK_GL_ERROR

	}

	void Shader::setUniformFloat(const std::string& name, float val)
	{
		bind();
		glUniform1f(getUniformLocation(name), val);		BANANA_CHECK_GL_ERROR
	}

	void Shader::setUniformFloat2(const std::string& name, float val1, float val2)
	{
		bind();
		glUniform2f(getUniformLocation(name), val1,val2);		BANANA_CHECK_GL_ERROR
	}

	void Shader::setUniformFloat2(const std::string& name, const glm::vec2& val)
	{
		setUniformFloat2(name, val.x, val.y);
	}

	void Shader::setUniformFloat3(const std::string& name, float val1, float val2, float val3)
	{
		bind();
		glUniform3f(getUniformLocation(name), val1, val2,val3);		BANANA_CHECK_GL_ERROR
	}

	void Shader::setUniformFloat3(const std::string& name, const glm::vec3& val)
	{
		setUniformFloat3(name, val.x, val.y, val.z);
	}

	void Shader::setUniformFloat4(const std::string& name, float val1, float val2, float val3, float val4)
	{
		bind();
		glUniform4f(getUniformLocation(name), val1, val2,val3,val4);		BANANA_CHECK_GL_ERROR
	}

	void Shader::setUniformFloat4(const std::string& name, const glm::vec4& val)
	{
		setUniformFloat4(name, val.x, val.y, val.z, val.w);
	}

	void Shader::setUniformMat3(const std::string& name, const glm::mat3& mat)
	{
		bind();
		glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));		BANANA_CHECK_GL_ERROR
	}

	void Shader::setUniformMat4(const std::string& name, const glm::mat4& mat)
	{
		bind();
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));		BANANA_CHECK_GL_ERROR
	}

	int Shader::getUniformLocation(const std::string& name)
	{
		if (m_uniformLocations.find(name) == m_uniformLocations.end())
		{
			m_uniformLocations[name] = glGetUniformLocation(m_ProgramId, name.c_str());		BANANA_CHECK_GL_ERROR
		}
		return m_uniformLocations[name];
	}
}