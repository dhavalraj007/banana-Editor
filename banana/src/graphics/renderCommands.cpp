#include"banana/graphics/renderCommands.h"
#include"banana/log.h"

#include"banana/graphics/mesh.h"
#include"banana/graphics/shader.h"

#include"glad/glad.h"

namespace banana::graphics::rendercommands
{
	void RenderMesh::execute()
	{
		std::shared_ptr<Mesh> mesh = m_Mesh.lock();
		std::shared_ptr<Shader> shader = m_Shader.lock();

		if (mesh && shader)
		{
			mesh->bind();
			shader->bind();

			if (mesh->getElementCount() > 0)
			{ 
				glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0);
			}
			else
			{
				glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->getVertexCount());
			}

			shader->unbind();
			mesh->unbind();
		}
		else
		{
			BANANA_WARN("Attempting execute RenderMesh with invalid data");
		}

	}
}