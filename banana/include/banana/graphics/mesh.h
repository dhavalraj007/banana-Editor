#pragma once

#include<cstdint>

namespace banana::graphics
{
	class Mesh
	{
	public:
		Mesh(float* vertexArray,uint32_t vertexCount,uint32_t dimensions);
		Mesh(float* vertexArray,uint32_t vertexCount,uint32_t dimensions,uint32_t* elementArray,uint32_t elementCount);
		Mesh(float* vertexArray,uint32_t vertexCount,uint32_t dimensions,float* texcoords,uint32_t* elementArray,uint32_t elementCount);
		~Mesh();

		void bind();
		void unbind();

		inline uint32_t getVertexCount() const { return m_vertexCount; }
		inline uint32_t getElementCount() const { return m_elementCount; }
	private:
		uint32_t m_Vao;
		uint32_t m_PositionVbo;
		uint32_t m_TexcoordsVbo;
		uint32_t m_Ebo; 
		uint32_t m_vertexCount,m_elementCount;
	};
}