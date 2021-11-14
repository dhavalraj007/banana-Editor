#include"banana/graphics/camera.h"
#include"banana/log.h"
#include"external/glm/gtc/matrix_transform.hpp"

namespace banana::graphics
{
	Camera2D::Camera2D()
		:m_Height(1080.f)
		,m_AspectRatio(16.f/9.f)
		,m_Near(-1.f)
		,m_Far(1.f)
		,m_ViewMatrix(1.f)
	{
		recalculateProjMat();
	}

	Camera2D::Camera2D(const Camera2D& other)
		:m_AspectRatio(other.m_AspectRatio)
		,m_Far(other.m_Far)
		,m_Near(other.m_Near)
		,m_Height(other.m_Height)
		, m_ViewMatrix(other.m_ViewMatrix)
	{
		recalculateProjMat();
	}

	void Camera2D::setHeight(float height)
	{
		if (m_Height != height)
		{
			m_Height = height;
			recalculateProjMat();
		}
	}

	void Camera2D::setNear(float _near)
	{
		if (m_Near != _near)
		{
			m_Near = _near;
			recalculateProjMat();
		}
	}

	void Camera2D::setFar(float _far)
	{
		if (m_Far = _far)
		{
			m_Far = _far;
			recalculateProjMat();
		}
	}

	void Camera2D::setAspectRatio(float ar)
	{
		if (m_AspectRatio != ar)
		{
			m_AspectRatio = ar;
			recalculateProjMat();
		}
	}

	void Camera2D::setOrtho(float height, float _near, float _far)
	{
		bool shouldRecalculate = false;
		if (m_Height != height)
		{
			m_Height = height;
			shouldRecalculate = true;
		}
		if (m_Near != _near)
		{
			m_Near = _near;
			shouldRecalculate = true;
		}
		if (m_Far != _far)
		{
			m_Far = _far;
			shouldRecalculate = true;
		}
		if (shouldRecalculate)
		{
			recalculateProjMat();
		}
	}

	void Camera2D::setView(const glm::vec3& pos, float rot)
	{
		m_CameraPos = pos;
		glm::mat4 view(1.f); // view = inverse(t*r*I) ;
		view = glm::translate(view,pos);
		view = glm::rotate(view,glm::radians(rot),glm::vec3(0.f,0.f,1.f));
		view = glm::inverse(view);
		setView(view);
	}

	void Camera2D::setView(const glm::vec3& pos, const glm::vec3& rot)
	{
		m_CameraPos = pos;
		glm::mat4 view(1.f); // view = inverse(t*r*I) ;
		view = glm::translate(view, pos);
		view = glm::rotate(view, glm::radians(rot.z), glm::vec3(0.f, 0.f, 1.f));
		view = glm::rotate(view, glm::radians(rot.y), glm::vec3(0.f, 1.f, 0.f));
		view = glm::rotate(view, glm::radians(rot.x), glm::vec3(1.f, 0.f, 0.f));
		view = glm::inverse(view);
		setView(view);
	}

	void Camera2D::recalculateProjMat()
	{
		float halfwidth = m_Height * m_AspectRatio * 0.5f;
		float halfheight = m_Height * 0.5f;
		m_ProjectionMatrix = glm::ortho(-halfwidth, halfwidth, -halfheight, halfheight, m_Near, m_Far);
	}

}