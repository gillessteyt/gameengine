#include "Helpers.h"
#include "RigidBodyConvexCylinderObject.h"
#include "Shader.h"
#include <gl/GLU.h>

#include <btBulletDynamicsCommon.h>

RigidBodyConvexCylinderObject::RigidBodyConvexCylinderObject(int i, Shader* shader, glm::vec3 position, float radius,float height, float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld) : RigidBodyObject(i, shader, mass)
{
	btCylinderShape* cylinder = new btCylinderShape({ radius,height / 2.0f,radius });
	m_Body = HELPERS::addConvexBody(cylinder, shader, gWorld, m_Mass, position, shouldAddToWorld);
	m_Body->setRollingFriction(1);

	float y = ((btCylinderShape*)m_Body->getCollisionShape())->getHalfExtentsWithoutMargin().getY();
	m_Transform.SetPivotOffset(glm::vec3(0.0f, -y, 0.0f));

	m_Body->setUserPointer(this);
}

RigidBodyConvexCylinderObject::~RigidBodyConvexCylinderObject()
{
}

void RigidBodyConvexCylinderObject::Draw(GLUquadric* gQuad, Camera* camera)
{
	RigidBodyObject::Draw(gQuad, camera);

	btTransform t;
	m_Body->getMotionState()->getWorldTransform(t);
	m_Transform.SetTransform(&t);
	m_Shader->Update(m_Transform, camera);

	btVector3 halfExtents = ((btCylinderShape*)m_Body->getCollisionShape())->getHalfExtentsWithoutMargin();

	gluCylinder(gQuad, halfExtents.x(), halfExtents.z(), halfExtents.y() * 2, 20, 20);
}

