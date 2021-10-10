#pragma once
#include"banana/graphics/renderCommands.h"
#include<queue>

namespace banana::managers
{
	class RenderManager
	{
	public:
		RenderManager() {}
		~RenderManager() {}

		void initialize();
		void shutdown();

		void clear();
		void setClearColor(float r, float g, float b, float a);
		void setWireframeMode(bool value);
		void submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rendercommand);
		
		//execute the rendercommands in the order they were received 
		void flush();
	private:
		std::queue<std::unique_ptr<graphics::rendercommands::RenderCommand>> m_RenderCommands;
	};
}