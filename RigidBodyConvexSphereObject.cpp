#include "Helpers.h"
#include "RigidBodyConvexSphereObject.h"
#include "Shader.h"
#include <gl/GLU.h>

#include <btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>

RigidBodyConvexSphereObject::RigidBodyConvexSphereObject(int i, Shader* shader, glm::vec3 position, float radius, float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld) : RigidBodyObject(i, shader, mass)
{
	btSphereShape* sphere = new btSphereShape(radius);
	m_Body = HELPERS::addConvexBody(sphere, shader, gWorld, m_Mass, position, shouldAddToWorld);
	m_Body->setRollingFriction(1);

	m_Body->setUserPointer(this);
}

RigidBodyConvexSphereObject::~RigidBodyConvexSphereObject()
{
}

void RigidBodyConvexSphereObject::Draw(GLUquadric* gQuad, Camera* camera)
{
	RigidBodyObject::Draw(gQuad, camera);

	btTransform t;
	m_Body->getMotionState()->getWorldTransform(t);
	m_Transform.SetTransform(&t);
	m_Shader->Update(m_Transform, camera);

	glColor3f(1, 0, 0);
	float rad = ((btSphereShape*)m_Body->getCollisionShape())->getRadius();

	gluSphere(gQuad, rad, 20, 20);
}

