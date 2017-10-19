#pragma once
#include "GameObject.h"
class RigidBodyObject :
	public GameObject
{
public:
	RigidBodyObject(int i, Shader* shader, float mass);
	virtual ~RigidBodyObject();

	virtual btRigidBody* Body() const { return m_Body; }
	virtual void Draw(GLUquadric* gQuad, Camera* camera) = 0;

protected:
	btRigidBody* m_Body;
	float m_Mass = 0.0f;
};

