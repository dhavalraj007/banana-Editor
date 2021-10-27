#pragma once
#include<cstdint>
#include"external/glm/glm.hpp"
namespace banana::graphics
{
	class Framebuffer
	{
	public:
		Framebuffer(uint32_t width, uint32_t height);
		~Framebuffer();

		inline uint32_t getFbo() const { return m_Fbo; }
		inline uint32_t getTextureId() const { return m_TextureId; }
		inline uint32_t getRenderBufferId() const { return m_RenderBufferId; }

		inline const glm::ivec2& getSize() { return m_size; };
		inline void SetClearColor(const glm::vec4& cc) { m_clearColor = cc; };
		inline const glm::vec4& GetClearColor() { return m_clearColor; }
	private:
		uint32_t m_Fbo;
		uint32_t m_TextureId;			// color attachment
		uint32_t m_RenderBufferId;		// depth and stencil attachment
		glm::ivec2 m_size;
		glm::vec4 m_clearColor;
	};
}