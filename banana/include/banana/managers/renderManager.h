#pragma once
#include"banana/graphics/renderCommands.h"
#include"external/glm/glm.hpp"
#include<queue>
#include<stack>

#define BANANA_SUBMIT_RC(cmdtype,...) std::move(std::make_unique<banana::graphics::rendercommands::cmdtype>(__VA_ARGS__))
#define BANANA_CRETE_VERTEX_BUFFER(name,type) std::unique_ptr<banana::graphics::VertexBuffer<type>> name = std::make_unique<banana::graphics::VertexBuffer<type>>();

namespace banana::managers
{
	class RenderManager
	{
		friend class graphics::rendercommands::PushFramebuffer;
		friend class graphics::rendercommands::PopFramebuffer;
		friend class graphics::rendercommands::PushCamera;
		friend class graphics::rendercommands::PopCamera;
	public:
		RenderManager() {}
		~RenderManager() {}

		const graphics::Camera2D* getActiveCamera() const;

		void initialize();
		void shutdown();

		void clear();
		void setViewport(int x, int y, int w, int h);
		void setClearColor(const glm::vec4& clearColor);
		void setWireframeMode(bool value);
		void submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rendercommand);
		
		//execute the rendercommands in the order they were received 
		void flush();

	
	private:
		std::queue<std::unique_ptr<graphics::rendercommands::RenderCommand>> m_RenderCommands;
		std::stack<std::shared_ptr<graphics::Framebuffer>> m_FrameBuffers;
		std::stack<std::shared_ptr<graphics::Camera2D>> m_Cameras;
		
		// no one can call these execpt graphics::rendercommands::PushFramebuffer and PopFramebuffer
		void pushFrameBuffer(std::shared_ptr<graphics::Framebuffer> framebuffer);		
		void popFramebuffer();

		void pushCamera(std::shared_ptr<graphics::Camera2D> camera);		
		void popCamera();
	};
}