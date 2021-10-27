#include"banana/graphics/framebuffer.h"
#include"banana/log.h"
#include"banana/graphics/helper.h"
#include"glad/glad.h"
namespace banana::graphics
{
	Framebuffer::Framebuffer(uint32_t width, uint32_t height)
		:m_Fbo(0),
		m_TextureId(0),
		m_RenderBufferId(0),
		m_size({width,height}),
		m_clearColor(glm::vec4(1.f))
	{
		glGenFramebuffers(1, &m_Fbo);	BANANA_CHECK_GL_ERROR
		glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);	BANANA_CHECK_GL_ERROR

		//create color tex attachement
		glGenTextures(1, &m_TextureId);	BANANA_CHECK_GL_ERROR;
		glBindTexture(GL_TEXTURE_2D, m_TextureId);	BANANA_CHECK_GL_ERROR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE,nullptr);	BANANA_CHECK_GL_ERROR//allocate mem
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); BANANA_CHECK_GL_ERROR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); BANANA_CHECK_GL_ERROR
		glBindTexture(GL_TEXTURE_2D, 0); BANANA_CHECK_GL_ERROR

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureId,0); BANANA_CHECK_GL_ERROR

		// create depth,stencil attachement
		glGenRenderbuffers(1, &m_RenderBufferId); BANANA_CHECK_GL_ERROR
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId); BANANA_CHECK_GL_ERROR
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_size.x, m_size.y); BANANA_CHECK_GL_ERROR
		glBindRenderbuffer(GL_RENDERBUFFER, 0); BANANA_CHECK_GL_ERROR

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferId); BANANA_CHECK_GL_ERROR

		uint32_t completeStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER); BANANA_CHECK_GL_ERROR
		if (completeStatus != GL_FRAMEBUFFER_COMPLETE)	
		{
			BANANA_ERROR("Failure to create Framebuffer Comple status is {}", completeStatus);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0); BANANA_CHECK_GL_ERROR
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_Fbo); 
		m_Fbo = 0;
		m_TextureId = 0;
		m_RenderBufferId = 0;
	}

}
