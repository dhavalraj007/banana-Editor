#include"banana/graphics/renderCommands.h"
#include"banana/log.h"
#include"banana/engine.h"
#include<algorithm>
#include<vector>

#include"banana/graphics/mesh.h"
#include"banana/graphics/texture.h"
#include"banana/graphics/shader.h"
#include"banana/graphics/framebuffer.h"

#include"glad/glad.h"

namespace banana::graphics
{
	std::vector<std::weak_ptr<Texture>> banana::graphics::multipleTexures(std::initializer_list<std::shared_ptr<Texture>> sTexs)
	{
		std::vector<std::weak_ptr<Texture>> ret(sTexs.size(), std::weak_ptr<Texture>());
		std::transform(begin(sTexs), end(sTexs), begin(ret), [](auto&& stex) {return std::weak_ptr<Texture>(stex); });
		return ret;
	}
}

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

	void RenderTexturedMesh::execute()
	{
		std::shared_ptr<Mesh> mesh = m_Mesh.lock();
		std::shared_ptr<Texture> texture = m_Texture.lock();
		std::shared_ptr<Shader> shader = m_Shader.lock();

		if (mesh && texture && shader)
		{
			mesh->bind();
			texture->bind();
			shader->bind();
			shader->setUniformInt(texture->getName(),texture->getTexUnit());
			if (mesh->getElementCount() > 0)
			{
				glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0);
			}
			else
			{
				glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->getVertexCount());
			}

			shader->unbind();
			texture->unbind();
			mesh->unbind();
		}
		else
		{
			BANANA_WARN("Attempting execute RenderMesh with invalid data");
		}
	}
	void RenderMultiTexturedMesh::execute()
	{
		std::shared_ptr<Mesh> mesh = m_Mesh.lock();
		std::vector<std::shared_ptr<Texture>> sTextures;
		std::for_each(begin(m_Textures), end(m_Textures),
			[&sTextures](auto&& wtextures)																	{ sTextures.emplace_back(wtextures.lock()); }
		);
		std::shared_ptr<Shader> shader = m_Shader.lock();
		bool tex_alive = std::all_of(begin(sTextures), end(sTextures), [](auto&& tex) { return tex; });
		if (mesh && tex_alive && shader)
		{
			mesh->bind();
			shader->bind();
			std::for_each(begin(sTextures), end(sTextures),
			[&shader](auto&& stex) 
			{ 
				stex->bind();
				shader->setUniformInt(stex->getName(), stex->getTexUnit());
				} );
			
			if (mesh->getElementCount() > 0)
			{
				glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0);
			}
			else
			{
				glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->getVertexCount());
			}

			shader->unbind();
			std::for_each(begin(sTextures), end(sTextures),
				[&shader](auto&& stex)
				{
					stex->unbind();
				});
			mesh->unbind();
		}
		else
		{
			BANANA_WARN("Attempting execute RenderMesh with invalid data");
		}
	}

	void PushFramebuffer::execute()
	{
		std::shared_ptr<Framebuffer> framebuffer = m_Framebuffer.lock();
		if (framebuffer)
		{
			Engine::Instance().getRenderManager().pushFrameBuffer(framebuffer);
		}
		else
		{
			BANANA_WARN("Attempting execute PushFramebuffer with invalid data");
		}
	}

	void PopFramebuffer::execute()
	{
		Engine::Instance().getRenderManager().popFramebuffer();

	}
	
	
}