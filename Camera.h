#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "Helpers.h"

class Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar, float initRot) : m_Position(pos), m_OrientationX(initRot)
	{
		m_Perspective = glm::perspective(fov, aspect, zNear, zFar);
		m_Forward = glm::vec3(cos(m_OrientationX), 0, -sin(m_OrientationX));
		//m_OrientationX = atan2f(m_Forward.x, -m_Forward.z);
		m_Orientation.y = m_OrientationX;
		m_Up = glm::vec3(0, 1, 0);
	}
	~Camera() {}

	void Move(glm::vec3& direction)
	{
		m_Position += direction;
	}

	void RotateY(float y)
	{
		btVector3 fwd = m_Forward;
		m_Forward = HELPERS::ToGLMVec(&fwd.rotate(glm::vec3(0, 1, 0), y));

		btVector3 up = m_Up;
		m_Up = HELPERS::ToGLMVec(&up.rotate(glm::vec3(0, 1, 0), y));

	}

	void RotateX(float x)
	{
		btVector3 up = m_Up;
		m_Up = HELPERS::ToGLMVec(&up.rotate(GetRight(), x));
		
		btVector3 fwd = m_Forward;
		m_Forward = HELPERS::ToGLMVec(&fwd.rotate(GetRight(), x));
	}

	glm::vec3 GetRight() const
	{
		return glm::cross(m_Forward, m_Up);
	}

	glm::vec3 GetUp() const
	{
		return m_Up;
	}

	glm::vec3 GetPosition() const
	{
		return m_Position;
	}

	glm::vec3 GetForward() const
	{
		return m_Forward;
	}

	glm::mat4 GetViewProjection() const
	{
		return m_Perspective * glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	}

private:
	glm::vec3 m_Orientation;
	float m_OrientationX = 0;
	glm::mat4 m_Perspective;
	glm::vec3 m_Position;
	glm::vec3 m_Forward;
	glm::vec3 m_Up;
};
