#include "gameobject.h"
#include"banana/engine.h"

#include"external/glm/gtc/matrix_transform.hpp"


void GameObject::Update()
{
	
	m_Model = glm::mat4(1.f);
	m_Model = glm::translate(m_Model, { m_Pos.x,m_Pos.y,0.f });
	m_Model = glm::scale(m_Model, { m_Size.x,m_Size.y,0.f });
	m_Shader->setUniformMat4("model", m_Model);
	static int frames = 0;
	frames++;
	m_Shader->setUniformFloat("iTime", frames * 0.0001f);
}

void GameObject::Render()
{
	
	banana::Engine::Instance().getRenderManager().submit(BANANA_SUBMIT_RC(RenderVertexArray, m_va, m_Shader));
}
