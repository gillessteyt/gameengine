#include "Helpers.h"
#include "RigidBodyConvexMeshObject.h"
#include "Shader.h"
#include <gl/GLU.h>

#include <btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include "Mesh.h"
#include <BulletCollision/CollisionShapes/btShapeHull.h>

RigidBodyConvexMeshObject::RigidBodyConvexMeshObject(int i, Shader* shader, glm::vec3 position, Mesh* mesh, float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld) : RigidBodyObject(i, shader, mass)
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

	btConvexShape* convexShape = new btConvexTriangleMeshShape(meshInterface);

	btShapeHull* convexMesh = new btShapeHull(convexShape);

	btScalar margin = convexShape->getMargin();
	convexMesh->buildHull(margin);

	btConvexHullShape* hullShape = new btConvexHullShape();

	auto convexVertices = convexMesh->getVertexPointer();

	for (int i = 0; i < convexMesh->numVertices(); ++i)
	{
		hullShape->addPoint(convexVertices[i]);
	}

	m_Body = HELPERS::addConvexBody(hullShape, shader, gWorld, mass, position, shouldAddToWorld);

	m_Mesh = mesh;
	m_Body->setUserPointer(this);

	delete meshInterface;
	delete convexShape;
	delete convexMesh;
}

RigidBodyConvexMeshObject::~RigidBodyConvexMeshObject()
{
}

void RigidBodyConvexMeshObject::Draw(GLUquadric* gQuad, Camera* camera)
{
	RigidBodyObject::Draw(gQuad, camera);

	btTransform t;
	m_Body->getMotionState()->getWorldTransform(t);

	m_Transform.SetTransform(&t, false);

	m_Shader->Update(m_Transform, camera);

	m_Mesh->Draw();
}

