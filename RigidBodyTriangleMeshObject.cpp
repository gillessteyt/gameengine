#include "Helpers.h"
#include "RigidBodyTriangleMeshObject.h"
#include "Shader.h"
#include <gl/GLU.h>

#include <btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include "Mesh.h"

RigidBodyTriangleMeshObject::RigidBodyTriangleMeshObject(int i, Shader* shader, glm::vec3 position, Mesh* mesh, float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld) : RigidBodyObject(i, shader, mass)
{
	btTriangleMesh* meshInterface = new btTriangleMesh();
	btAlignedObjectArray<btVector3> convertedVerts;

	std::vector<Vertex> vertices = mesh->GetVertices();
	std::vector<unsigned int> indices = mesh->GetIndices();
	unsigned int nrOfVertices = vertices.size();
	unsigned int nrOfIndices = indices.size();

	convertedVerts.reserve(nrOfVertices);

	for (int i = 0; i < nrOfVertices; i++)
	{
		convertedVerts.push_back(btVector3(vertices[i].Position()));
	}

	for (int i = 0; i < nrOfIndices / 3; i++)
	{
		const btVector3& v0 = convertedVerts[indices[i * 3]];
		const btVector3& v1 = convertedVerts[indices[i * 3 + 1]];
		const btVector3& v2 = convertedVerts[indices[i * 3 + 2]];

		meshInterface->addTriangle(v0, v1, v2);
	}

	btBvhTriangleMeshShape* triangleMesh = new btBvhTriangleMeshShape(meshInterface, true, true);

	m_Body = HELPERS::addTriangleBody(triangleMesh, shader, gWorld, mass, position, shouldAddToWorld);
	m_Mesh = mesh;
	m_Body->setUserPointer(this);

	delete meshInterface;
}

RigidBodyTriangleMeshObject::~RigidBodyTriangleMeshObject()
{
	//if (m_Mesh) delete m_Mesh;
}

void RigidBodyTriangleMeshObject::Draw(GLUquadric* gQuad, Camera* camera)
{
	RigidBodyObject::Draw(gQuad, camera);

	btTransform t;
	m_Body->getMotionState()->getWorldTransform(t);
	
	m_Transform.SetTransform(&t, false);

	m_Shader->Update(m_Transform, camera);

	m_Mesh->Draw();
}

