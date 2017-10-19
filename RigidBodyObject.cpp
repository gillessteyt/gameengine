#include "RigidBodyObject.h"
#include <btBulletDynamicsCommon.h>
#include "Helpers.h"
#include "Shader.h"


RigidBodyObject::RigidBodyObject(int i, Shader* shader, float mass) : GameObject(i, shader), m_Mass(mass), m_Body(nullptr)
{
	m_IsSoftBody = false;
}

RigidBodyObject::~RigidBodyObject()
{
	delete m_Body->getMotionState();
	delete m_Body->getCollisionShape();
	delete m_Body;
}

void RigidBodyObject::Draw(GLUquadric* gQuad, Camera* camera)
{
	m_Shader->Bind();
	m_Shader->SetColor(m_Color);
	m_Shader->SetSpecular(m_Specular);
}

