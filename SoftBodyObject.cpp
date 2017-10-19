#include "SoftBodyObject.h"
#include "Helpers.h"
#include "Shader.h"

SoftBodyObject::SoftBodyObject(btSoftBody* body, int i, Shader* shader) : GameObject(i, shader), m_Body(body)
{
	m_IsSoftBody = true;

}

SoftBodyObject::~SoftBodyObject()
{
	delete m_Body;
}

void SoftBodyObject::Draw(GLUquadric* gQuad, Camera* camera)
{
	btTransform t;

	t = m_Body->getWorldTransform();
	m_Transform.SetTransform(&t, false);

	m_Shader->Update(m_Transform, camera);
	HELPERS::renderSoftBody(m_Body, m_Shader);
}

