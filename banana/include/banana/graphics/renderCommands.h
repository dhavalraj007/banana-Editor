#pragma once
#include<memory>

namespace banana::graphics
{
	class Mesh;
	class Shader;
	class Framebuffer;
	namespace rendercommands
	{
		class RenderCommand 
		{
		public:
			virtual void execute() = 0;
			virtual ~RenderCommand() {}
		};

		class RenderMesh :public RenderCommand
		{
		public:
			RenderMesh(std::weak_ptr<Mesh> mesh, std::weak_ptr<Shader> shader)
			:m_Mesh(mesh),m_Shader(shader)
			{}

			virtual void execute() override;
		private:
			std::weak_ptr<Mesh> m_Mesh;
			std::weak_ptr<Shader> m_Shader;
		};

		class PushFramebuffer :public RenderCommand
		{
		public:
			PushFramebuffer(std::weak_ptr<Framebuffer> framebuffer):m_Framebuffer(framebuffer){}
			virtual void execute() override;
		private:
			std::weak_ptr<Framebuffer> m_Framebuffer;
		};

		class PopFramebuffer :public RenderCommand
		{
		public:
			PopFramebuffer(){}
			virtual void execute() override;
		};
	}
}