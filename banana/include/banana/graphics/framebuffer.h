#pragma once
#include<cstdint>

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
		inline void getSize(uint32_t& width, uint32_t& height) { width = m_Width; height = m_Height; };
		inline void SetClearColor(float r, float g, float b, float a) { m_CCR = r;  m_CCG = g; m_CCB = b; m_CCA = a;};
		inline void GetClearColor(float& r, float& g, float& b, float& a) { r = m_CCR; g = m_CCB; b = m_CCB; a = m_CCA; }
	private:
		uint32_t m_Fbo;
		uint32_t m_TextureId;			// color attachment
		uint32_t m_RenderBufferId;		// depth and stencil attachment
		uint32_t m_Width, m_Height;
		float m_CCR, m_CCG, m_CCB, m_CCA;
	};
}