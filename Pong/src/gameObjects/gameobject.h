#pragma once
#include"external/glm/glm.hpp"
#include<memory>
#include"banana/graphics/mesh.h"
#include"banana/graphics/shader.h"

class GameObject
{
public:
	GameObject(std::shared_ptr<banana::graphics::Mesh> mesh,std::shared_ptr<banana::graphics::Shader> shader,const glm::vec2& pos,const glm::vec2& size)
		:m_Mesh(mesh)
		,m_Shader(shader)
		,m_Pos(pos)
		,m_Size(size)
		,m_Model(1.f)
	{}

	void setPos(const glm::vec2& pos) { m_Pos = pos; }
	void move(const glm::vec2& by) { m_Pos += by; }

	const glm::vec2& getPos() { return m_Pos; }
	const glm::vec2& getSize() { return m_Size; }

	virtual void Update();
	virtual void Render();
private:
	std::shared_ptr<banana::graphics::Mesh> m_Mesh;
	std::shared_ptr<banana::graphics::Shader> m_Shader;

	glm::vec2 m_Pos;
	glm::vec2 m_Size;
	glm::mat4 m_Model;
};