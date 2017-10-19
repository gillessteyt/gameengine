#include "Helpers.h"
#include "RigidBodyConvexBoxObject.h"
#include "Shader.h"
#include "Mesh.h"
#include <gl/GLU.h>

#include <btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>

RigidBodyConvexBoxObject::RigidBodyConvexBoxObject(int i, Shader* shader, glm::vec3 position, glm::vec3 extents, Mesh* meshBox, float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld) : RigidBodyObject(i, shader, mass)
{
	btBoxShape* box = new btBoxShape(btVector3(extents[0] / 2.0f, extents[1] / 2.0f, extents[2] / 2.0f));

	m_Body = HELPERS::addConvexBody(box, shader, gWorld, mass, position, shouldAddToWorld);

	m_Mesh = meshBox;
	m_Body->setRollingFriction(1);

	float y = ((btConeShape*)m_Body->getCollisionShape())->getHeight() / 2;
	m_Transform.SetPivotOffset(glm::vec3(0.0f, -y, 0.0f));
	m_Body->setUserPointer(this);
}

RigidBodyConvexBoxObject::~RigidBodyConvexBoxObject()
{
	//if (m_Mesh) delete m_Mesh;
}

void RigidBodyConvexBoxObject::Draw(GLUquadric* gQuad, Camera* camera)
{
	RigidBodyObject::Draw(gQuad, camera);

	btTransform t;
	m_Body->getMotionState()->getWorldTransform(t);

	m_Transform.SetTransform(&t, false);

	m_Shader->Update(m_Transform, camera);

	m_Mesh->Draw();
}

