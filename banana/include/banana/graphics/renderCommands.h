#pragma once
#include<memory>

namespace banana::graphics
{
	class Mesh;
	class Shader;
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

			virtual void execute();
		private:
			std::weak_ptr<Mesh> m_Mesh;
			std::weak_ptr<Shader> m_Shader;
		};
	}
}