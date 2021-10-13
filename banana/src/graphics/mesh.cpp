
#include"banana/graphics/mesh.h"
#include"glad/glad.h"

namespace banana::graphics
{
	Mesh::Mesh(float* vertexArray, uint32_t vertexCount, uint32_t dimensions)
		:m_vertexCount(vertexCount)
		,m_Ebo(0)
		,m_elementCount(0)
	{
		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

			glGenBuffers(1, &m_PositionVbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_PositionVbo);
				
				glBufferData(GL_ARRAY_BUFFER, vertexCount * dimensions * sizeof(float), vertexArray, GL_STATIC_DRAW);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
				glDisableVertexAttribArray(0);
		
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	Mesh::Mesh(float* vertexArray, uint32_t vertexCount, uint32_t dimensions, uint32_t* elementArray, uint32_t elementCount)
		:Mesh(vertexArray,vertexCount,dimensions)
	{
		m_elementCount = elementCount;

		glBindVertexArray(m_Vao);

			glGenBuffers(1, &m_Ebo); 
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_Ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(uint32_t), elementArray, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &m_PositionVbo);
		if (m_Ebo != 0)
		{
			glDeleteBuffers(1, &m_Ebo);
		}
		glDeleteVertexArrays(1, &m_Vao);
	}
	void Mesh::bind()
	{
		glBindVertexArray(m_Vao);
		glEnableVertexAttribArray(0);
	}
	void Mesh::unbind()
	{
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}