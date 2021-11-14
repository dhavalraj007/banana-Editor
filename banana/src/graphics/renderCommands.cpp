#include"banana/graphics/renderCommands.h"
#include"banana/log.h"
#include"banana/engine.h"
#include<algorithm>
#include<vector>

#include"banana/graphics/vertex.h"
#include"banana/graphics/texture.h"
#include"banana/graphics/shader.h"
#include"banana/graphics/framebuffer.h"
#include"banana/graphics/camera.h"

#include"glad/glad.h"

namespace banana::graphics
{
	//convert vec of shared ptr to Texture into weak_ptr to Texture
	std::vector<std::weak_ptr<Texture>> banana::graphics::multipleTexures(std::initializer_list<std::shared_ptr<Texture>> sTexs)
	{
		std::vector<std::weak_ptr<Texture>> ret(sTexs.size(), std::weak_ptr<Texture>());
		std::transform(begin(sTexs), end(sTexs), begin(ret), [](auto&& stex) {return std::weak_ptr<Texture>(stex); });
		return ret;
	}
}

namespace banana::graphics::rendercommands
{

	void RenderVertexArray::execute()
	{
		std::shared_ptr<VertexArray> va = m_Va.lock();
		std::shared_ptr<Shader> shader = m_Shader.lock();
		
		if (va && shader)
		{

			BANANA_ASSERT(va->isValid(), "Attempting to Call Invalid RenderVertexArray with invalid VertexArray. - did you Upload the VertexArray ?");
			if (va->isValid())
			{
				va->bind();
				shader->bind();

				const auto& rm = Engine::Instance().getRenderManager();
				const auto& cam = rm.getActiveCamera();
				
				if (cam)
				{
					shader->setUniformMat4("proj", cam->getProjectionMatrix());
					shader->setUniformMat4("view", cam->getViewMatrix());
				}

				if (va->getElementCount() > 0)
				{
					glDrawElements(GL_TRIANGLES, va->getElementCount(), GL_UNSIGNED_INT, 0);
				}
				else
				{
					glDrawArrays(GL_TRIANGLE_STRIP, 0, va->getVertexCount());
				}

				shader->unbind();
				va->unbind();
			}
		}
		else
		{
			BANANA_WARN("Attempting execute RenderVertexArray with invalid data");
		}
	}

	void RenderTexturedVertexArray::execute()
	{
		std::shared_ptr<VertexArray> va = m_Va.lock();
		std::shared_ptr<Texture> texture = m_Texture.lock();
		std::shared_ptr<Shader> shader = m_Shader.lock();

		if (va && texture && shader)
		{
			BANANA_ASSERT(va->isValid(), "Attempting to Call Invalid RenderTexturedVertexArray with invalid VertexArray. - did you Upload the VertexArray ?");
			if (va->isValid())
			{
				va->bind();
				texture->bind();
				shader->bind();

				const auto& rm = Engine::Instance().getRenderManager();
				const auto& cam = rm.getActiveCamera();

				if (cam)
				{
					shader->setUniformMat4("proj", cam->getProjectionMatrix());
					shader->setUniformMat4("view", cam->getViewMatrix());

				}
				shader->setUniformInt(texture->getName(), texture->getTexUnit());

				if (va->getElementCount() > 0)
				{
					glDrawElements(GL_TRIANGLES, va->getElementCount(), GL_UNSIGNED_INT, 0);
				}
				else
				{
					glDrawArrays(GL_TRIANGLES, 0, va->getVertexCount());
				}

				shader->unbind();
				texture->unbind();
				va->unbind();
			}
	 
		}
		else
		{
			BANANA_WARN("Attempting execute RenderTexturedVertexArray with invalid data");
		}
	}
	void RenderMultiTexturedVertexArray::execute()
	{
		std::shared_ptr<VertexArray> va = m_Va.lock();
		std::vector<std::shared_ptr<Texture>> sTextures;
		std::for_each(begin(m_Textures), end(m_Textures),
			[&sTextures](auto&& wtextures) { sTextures.emplace_back(wtextures.lock()); }
		);
		std::shared_ptr<Shader> shader = m_Shader.lock();

		bool tex_alive = std::all_of(begin(sTextures), end(sTextures), [](auto&& tex) { return tex; });
		if (va && tex_alive && shader)
		{
			BANANA_ASSERT(va->isValid(), "Attempting to Call Invalid RenderMultiTexturedVertexArray with invalid VertexArray. - did you Upload the VertexArray ?");
			if (va->isValid())
			{
				va->bind();
				shader->bind();

				const auto& rm = Engine::Instance().getRenderManager();
				const auto& cam = rm.getActiveCamera();

				if (cam)
				{
					shader->setUniformMat4("proj", cam->getProjectionMatrix());
					shader->setUniformMat4("view", cam->getViewMatrix());
				}


				std::for_each(begin(sTextures), end(sTextures),
					[&shader](auto&& stex)
					{
						stex->bind();
						shader->setUniformInt(stex->getName(), stex->getTexUnit());
					});

				if (va->getElementCount() > 0)
				{
					glDrawElements(GL_TRIANGLES, va->getElementCount(), GL_UNSIGNED_INT, 0);
				}
				else
				{
					glDrawArrays(GL_TRIANGLE_STRIP, 0, va->getVertexCount());
				}

				shader->unbind();
				std::for_each(begin(sTextures), end(sTextures),
					[&shader](auto&& stex)
					{
						stex->unbind();
					});
				va->unbind();
			}
		}
		else
		{
			BANANA_WARN("Attempting execute RenderMultiTexturedVertexArray with invalid data");
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

	void PushCamera::execute()
	{
		std::shared_ptr<Camera2D> camera = m_Camera.lock();
		if (camera)
		{
			Engine::Instance().getRenderManager().pushCamera(camera);
		}
		else
		{
			BANANA_WARN("Attempting execute PushCamera with invalid data");
		}
	}

	void PopCamera::execute()
	{
		Engine::Instance().getRenderManager().popCamera();
	}
}