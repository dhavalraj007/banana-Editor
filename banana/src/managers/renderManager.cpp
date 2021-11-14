#include "banana/managers/renderManager.h"
#include"banana/log.h"
#include"banana/engine.h"
#include"glad/glad.h"
#include"banana/graphics/framebuffer.h"
#include"banana/graphics/helper.h"


namespace banana::managers
{

	void RenderManager::initialize()
	{

		BANANA_INFO("OpenGL Info:\n Vendor:\t{}\n Renderer:\t{}\n Version\t{}",
			(const char*)glGetString(GL_VENDOR),
			(const char*)glGetString(GL_RENDERER),
			(const char*)glGetString(GL_VERSION));

		glEnable(GL_DEPTH_TEST);	BANANA_CHECK_GL_ERROR;
		glDepthFunc(GL_LEQUAL);	BANANA_CHECK_GL_ERROR;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		setClearColor(
			{ static_cast<float>(0x64) / static_cast<float>(0xFF),
			static_cast<float>(0x95) / static_cast<float>(0xFF),
			static_cast<float>(0xED) / static_cast<float>(0xFF), 
			1 } 
		);	// cornflower blue default clear color
	}

	void RenderManager::shutdown()
	{
		while (m_RenderCommands.size() > 0)
		{
			m_RenderCommands.pop();
		}
	}

	// clear frame and assert none of the rendercommands left in queue
	void RenderManager::clear()
	{
		BANANA_ASSERT(m_RenderCommands.size() == 0, " Unflushed RenderCommands in queue!");
		while (m_RenderCommands.size() > 0)
		{
			m_RenderCommands.pop();
		}
	}

	void RenderManager::setViewport(int x, int y, int w, int h)
	{
		glViewport(x, y, w, h);
	}

	void RenderManager::setClearColor(const glm::vec4& clearColor)
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	}

	void RenderManager::setWireframeMode(bool value)
	{
		if (value)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	}

	void RenderManager::submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rendercommand)
	{
		m_RenderCommands.push(std::move(rendercommand));
	}

	void RenderManager::flush()
	{
		while (m_RenderCommands.size() > 0)
		{
			auto rendercommand = std::move(m_RenderCommands.front());
			m_RenderCommands.pop();

			rendercommand->execute();
		}
	}

	void RenderManager::pushFrameBuffer(std::shared_ptr<graphics::Framebuffer> framebuffer)
	{
		BANANA_ASSERT(framebuffer, "framebuffer is null.")
		m_FrameBuffers.push(framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->getFbo());
		setViewport(0, 0, framebuffer->getSize().x, framebuffer->getSize().y);

		
		glClearColor(framebuffer->GetClearColor().r, framebuffer->GetClearColor().g, framebuffer->GetClearColor().b, framebuffer->GetClearColor().a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);	BANANA_CHECK_GL_ERROR;
	}

	void RenderManager::popFramebuffer()
	{
		BANANA_ASSERT(m_FrameBuffers.size() > 0, "RenderManger::popFramebuffer: Empty stack");
		if (m_FrameBuffers.size() > 0)
		{
			m_FrameBuffers.pop();
			if (m_FrameBuffers.size() > 0)
			{
				auto nextfb = m_FrameBuffers.top();
				glBindFramebuffer(GL_FRAMEBUFFER, nextfb->getFbo());
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);	BANANA_CHECK_GL_ERROR;
				setViewport(0, 0, nextfb->getSize().x, nextfb->getSize().y);
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);	BANANA_CHECK_GL_ERROR;
				auto& window = Engine::Instance().getWindow();
				setViewport(0, 0, window.getSize().x, window.getSize().y);
			}
		}
	}




	const graphics::Camera2D* RenderManager::getActiveCamera() const 
	{
		if (m_Cameras.size() > 0)
		{
			return m_Cameras.top().get();
		}
		return nullptr;
	}

	void RenderManager::pushCamera(std::shared_ptr<graphics::Camera2D> camera)
	{
		BANANA_ASSERT(camera,"camera is null.")
		m_Cameras.push(camera);
	}

	void RenderManager::popCamera()
	{
		BANANA_ASSERT(m_Cameras.size() > 0, "RenderManger::popCamera: Empty stack");
		if (m_Cameras.size() > 0)
		{
			m_Cameras.pop();
		}
	}
}