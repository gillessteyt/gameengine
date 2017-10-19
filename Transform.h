#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <LinearMath/btTransform.h>
#include <iostream>

class Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(),
		const glm::vec3& rot = glm::vec3(),
		const glm::vec3& scale = { 1, 1, 1 })
		: m_Position(pos), m_Rotation(rot), m_Scale(scale), m_PivotOffset(0, 0, 0)
	{
	}

	glm::mat4 GetWorldMatrix() const
	{
		glm::mat4 transMat = glm::translate(m_Position);

		glm::mat4 rotXMat = glm::rotate(m_Rotation.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMat = glm::rotate(m_Rotation.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZMat = glm::rotate(m_Rotation.z, glm::vec3(0, 0, 1));
		glm::mat4 rotMat = rotZMat * rotYMat * rotXMat;

		glm::mat4 scaleMat = glm::scale(m_Scale);

		return transMat * rotMat * scaleMat;
	}

	glm::vec3& Position() { return m_Position; }
	glm::vec3& PivotOffset() { return m_PivotOffset; }
	glm::vec3& Rotation() { return m_Rotation; }
	glm::vec3& Scale() { return m_Scale; }

	//CORRECT BOOL IS USED TO CORRECT Z-UP MISMATCH IN OPENGL
	void SetTransform(btTransform* transform, bool correct = true)
	{
		float mat[16];
		transform->getOpenGLMatrix(mat);

		btQuaternion quat = transform->getRotation();
		m_Position = glm::vec3(mat[12], mat[13], mat[14]);
		
		//Used to offset mesh to match physics
		if (m_PivotOffset.y != 0)
		{
			btVector3 pO = btVector3(m_PivotOffset.x, m_PivotOffset.y, m_PivotOffset.z);
			pO = pO.rotate(quat.getAxis(), quat.getAngle());
			m_Position += glm::vec3(pO.getX(), pO.getY(), pO.getZ());
		}

		//Correct Z-Up
		if (correct)
		{
			btQuaternion correction;
			correction.setRotation(btVector3(1, 0, 0), -3.1415 / 2);

			quat *= correction;
		}

		float rotX, rotY, rotZ;
		quat.getEulerZYX(rotZ, rotY, rotX);
		m_Rotation = glm::vec3(rotX, rotY, rotZ);
	}

	void SetPosition(glm::vec3& pos) { m_Position = pos; }
	void SetPivotOffset(glm::vec3& po) { m_PivotOffset = po; }
	void SetRotation(glm::vec3& rot) { m_Rotation = rot; }
	void SetScale(glm::vec3& scale) { m_Scale = scale; }
private:
	glm::vec3 m_PivotOffset;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
};

