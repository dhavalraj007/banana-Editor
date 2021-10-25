#pragma once
#include"banana/graphics/renderCommands.h"
#include<queue>
#include<stack>

#define BANANA_SUBMIT_RC(cmdtype,...) std::move(std::make_unique<banana::graphics::rendercommands::cmdtype>(__VA_ARGS__))


namespace banana::managers
{
	class RenderManager
	{

		friend class graphics::rendercommands::PushFramebuffer;
		friend class graphics::rendercommands::PopFramebuffer;
	public:
		RenderManager() {}
		~RenderManager() {}

		void initialize();
		void shutdown();

		void clear();
		void setViewport(int x, int y, int w, int h);
		void setClearColor(float r, float g, float b, float a);
		void setWireframeMode(bool value);
		void submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rendercommand);
		
		//execute the rendercommands in the order they were received 
		void flush();

	
	private:
		std::queue<std::unique_ptr<graphics::rendercommands::RenderCommand>> m_RenderCommands;
		std::stack<std::shared_ptr<graphics::Framebuffer>> m_FrameBuffers;
		
		// no one can call these execpt graphics::rendercommands::PushFramebuffer and PopFramebuffer
		void pushFrameBuffer(std::shared_ptr<graphics::Framebuffer> framebuffer);		
		void popFramebuffer();
	};
}