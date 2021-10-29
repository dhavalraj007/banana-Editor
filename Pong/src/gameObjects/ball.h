#pragma once
#include"gameobject.h"

class Ball : public GameObject
{
public:
	Ball(std::shared_ptr<banana::graphics::Mesh> mesh, std::shared_ptr<banana::graphics::Shader> shader, const glm::vec2& pos, const glm::vec2& size)
		:GameObject(mesh,shader,pos,size)
		,m_Vel(0.f)
	{}
	
	void setVelocity(const glm::vec2& velocity) { m_Vel = velocity; }
	const glm::vec2& getVel() { return m_Vel; }

	void flipVelX() { m_Vel.x *= -1; }
	void flipVelY() { m_Vel.y *= -1; }
private:
	glm::vec2 m_Vel;
};
