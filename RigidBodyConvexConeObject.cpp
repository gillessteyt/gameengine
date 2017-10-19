#include "Helpers.h"
#include "RigidBodyConvexConeObject.h"
#include "Shader.h"
#include <gl/GLU.h>

#include <btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>

RigidBodyConvexConeObject::RigidBodyConvexConeObject(int i, Shader* shader, glm::vec3 position, float radius,float height, float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld) : RigidBodyObject(i, shader, mass)
{
	btConeShape* cylinder = new btConeShape({ radius,height  });
	m_Body = HELPERS::addConvexBody(cylinder, shader, gWorld, m_Mass, position, shouldAddToWorld);
	m_Body->setRollingFriction(1);

	float y = ((btConeShape*)m_Body->getCollisionShape())->getHeight() / 2;
	m_Transform.SetPivotOffset(glm::vec3(0.0f, -y, 0.0f));
	m_Body->setUserPointer(this);
}

RigidBodyConvexConeObject::~RigidBodyConvexConeObject()
{
}

void RigidBodyConvexConeObject::Draw(GLUquadric* gQuad, Camera* camera)
{
	RigidBodyObject::Draw(gQuad, camera);

	btTransform t;
	m_Body->getMotionState()->getWorldTransform(t);
	m_Transform.SetTransform(&t);
	m_Shader->Update(m_Transform, camera);
	
	btConeShape* coneShape = (btConeShape*)m_Body->getCollisionShape();
	float rad = coneShape->getRadius();
	float height = coneShape->getHeight();

	gluCylinder(gQuad, rad, 0, height, 20, 20);
}

