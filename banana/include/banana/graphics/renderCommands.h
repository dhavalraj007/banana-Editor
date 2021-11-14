#pragma once
#include<memory>
#include<vector>

namespace banana::graphics
{
	class VertexArray;
	class Texture;
	class Shader;
	class Framebuffer;
	class Camera2D;

	std::vector<std::weak_ptr<Texture>> multipleTexures(std::initializer_list<std::shared_ptr<Texture>> sTexs);

	namespace rendercommands
	{
		class RenderCommand 
		{
		public:  
			virtual void execute() = 0;
			virtual ~RenderCommand() {}
		};
		 
		class RenderVertexArray:public RenderCommand
		{
		public:
			RenderVertexArray(std::weak_ptr<VertexArray> va, std::weak_ptr<Shader> shader)
				:m_Va(va), m_Shader(shader)
			{}

			virtual void execute() override;
		private:
			std::weak_ptr<VertexArray> m_Va;
			std::weak_ptr<Shader> m_Shader;
		};

		class RenderTexturedVertexArray:public RenderCommand
		{
		public:
			RenderTexturedVertexArray(std::weak_ptr<VertexArray> va, std::weak_ptr<Texture> texture, std::weak_ptr<Shader> shader)
				:m_Va(va)
				, m_Texture(texture)
				, m_Shader(shader)
			{}

			virtual void execute() override;
		private:
			std::weak_ptr<VertexArray> m_Va;
			std::weak_ptr<Texture> m_Texture;
			std::weak_ptr<Shader> m_Shader;
		};

		class RenderMultiTexturedVertexArray :public RenderCommand
		{
		public:
			RenderMultiTexturedVertexArray(std::weak_ptr<VertexArray> va, const std::vector<std::weak_ptr<Texture>>& textures, std::weak_ptr<Shader> shader)
				:m_Va(va)
				, m_Textures(textures)
				, m_Shader(shader)
			{}

			virtual void execute() override;
		private:
			std::weak_ptr<VertexArray> m_Va;
			std::vector<std::weak_ptr<Texture>> m_Textures;
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

		class PushCamera :public RenderCommand
		{
		public:
			PushCamera(std::weak_ptr<Camera2D> camera) :m_Camera(camera) {}
			virtual void execute() override;
		private:
			std::weak_ptr<Camera2D> m_Camera;
		};

		class PopCamera :public RenderCommand
		{
		public:
			PopCamera() {}
			virtual void execute() override;
		};
	}
}