#pragma once
#include<memory>
#include<vector>

namespace banana::graphics
{
	class Mesh;
	class Texture;
	class Shader;
	class Framebuffer;

	std::vector<std::weak_ptr<Texture>> multipleTexures(std::initializer_list<std::shared_ptr<Texture>> sTexs);

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

		class RenderTexturedMesh :public RenderCommand
		{
		public:
			RenderTexturedMesh(std::weak_ptr<Mesh> mesh, std::weak_ptr<Texture> texture, std::weak_ptr<Shader> shader)
				:m_Mesh(mesh)
				, m_Texture(texture)
				, m_Shader(shader)		
			{}

			virtual void execute() override;
		private:
			std::weak_ptr<Mesh> m_Mesh;
			std::weak_ptr<Texture> m_Texture;
			std::weak_ptr<Shader> m_Shader;
		};
		class RenderMultiTexturedMesh :public RenderCommand
		{
		public:
			RenderMultiTexturedMesh(std::weak_ptr<Mesh> mesh, const std::vector<std::weak_ptr<Texture>>& textures, std::weak_ptr<Shader> shader)
				:m_Mesh(mesh)
				, m_Textures(textures)
				, m_Shader(shader)		
			{}

			virtual void execute() override;
		private:
			std::weak_ptr<Mesh> m_Mesh;
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
	}
}