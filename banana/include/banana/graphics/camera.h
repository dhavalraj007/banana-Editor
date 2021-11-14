#pragma once
#include<string>
#include"external/glm/glm.hpp"

namespace banana::graphics
{
	class Camera2D
	{
	public:
		Camera2D();
		Camera2D(const Camera2D& other);
		~Camera2D(){}
		
		float getHeight() const { return m_Height; }
		float getNear() const { return m_Near; }
		float getFar() const { return m_Far; }
		float getAspectRatio() const { return m_AspectRatio; }
		const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		
		void setHeight(float height);
		void setNear(float _near);
		void setFar(float _far);
		void setAspectRatio(float ar);
		void setOrtho(float height, float _near = 0.0f, float _far = 100.f);
		void setView(const glm::vec3& pos,float rot);
		void setView(const glm::vec3& pos,const glm::vec3& rot);
		inline void setView(const glm::mat4& viewMat) { m_ViewMatrix = viewMat; }

		void recalculateProjMat();

	protected:
		float m_AspectRatio;
		float m_Height,m_Near,m_Far;
		glm::vec3 m_CameraPos;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
	};

	
}