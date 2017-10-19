#include "Helpers.h"
#include "RigidBodyPlaneObject.h"
#include "Shader.h"
#include <gl/GLU.h>

#include <btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>

RigidBodyPlaneObject::RigidBodyPlaneObject(int i, Shader* shader, glm::vec3 position, btDynamicsWorld* gWorld, bool shouldAddToWorld) : RigidBodyObject(i, shader, 0)
{
	btTransform bulletTransform;
	bulletTransform.setIdentity();
	bulletTransform.setOrigin(btVector3(0, 0, 0));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btMotionState* motion = new btDefaultMotionState(bulletTransform);
	btRigidBody::btRigidBodyConstructionInfo info(0, motion, plane);
	m_Body = new btRigidBody(info);
	gWorld->addRigidBody(m_Body);

	SetColor(glm::vec4(1, 1, 1, 1));

	m_Body->setRollingFriction(1);

	m_Body->setUserPointer(this);
}

RigidBodyPlaneObject::~RigidBodyPlaneObject()
{
}

void RigidBodyPlaneObject::Draw(GLUquadric* gQuad, Camera* camera)
{
	RigidBodyObject::Draw(gQuad, camera);

	btTransform t;
	m_Body->getMotionState()->getWorldTransform(t);
	m_Transform.SetTransform(&t,false);
	m_Shader->Update(m_Transform, camera);

	glBegin(GL_QUADS);
	glVertex3f(20, 0, 20);
	glNormal3f(0, 1, 0);
	glVertex3f(20, 0, -20);
	glNormal3f(0, 1, 0);
	glVertex3f(-20, 0, -20);
	glNormal3f(0, 1, 0);
	glVertex3f(-20, 0, 20);
	glNormal3f(0, 1, 0);
	glEnd();
}

