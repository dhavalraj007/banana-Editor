#include "banana/managers\renderManager.h"
#include"banana/log.h"
#include"glad/glad.h"


namespace banana::managers
{



	void RenderManager::initialize()
	{

		BANANA_INFO("OpenGL Info:\n Vendor:\t{}\n Renderer:\t{}\n Version\t{}",
			(const char*)glGetString(GL_VENDOR),
			(const char*)glGetString(GL_RENDERER),
			(const char*)glGetString(GL_VERSION));

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		setClearColor(
			static_cast<float>(0x64) / static_cast<float>(0xFF),
			static_cast<float>(0x95) / static_cast<float>(0xFF),
			static_cast<float>(0xED) / static_cast<float>(0xFF),
			1
		);	// cornflower blue default clear color
	}

	void RenderManager::shutdown()
	{
		while (m_RenderCommands.size() > 0)
		{
			m_RenderCommands.pop();
		}
	}

	// clear previous frames
	void RenderManager::clear()
	{
		BANANA_ASSERT(m_RenderCommands.size()==0," Unflushed RenderCommands in queue!")
		while (m_RenderCommands.size() > 0)
		{
			m_RenderCommands.pop();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderManager::setClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
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
}