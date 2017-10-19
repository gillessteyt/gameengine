#pragma once
#include <btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

#include "BulletCollision/CollisionShapes/btShapeHull.h"

#include <vector>
#include <glew/glew.h>
#include "GameObject.h"
#include "Shader.h"
#include "Mesh.h"
#include "SoftBodyObject.h"
#include "RigidBodyObject.h"
#include "Helpers.h"

std::vector<std::pair<float, GameObject*>> HELPERS::GetObjectsInRadiusFromPoint(btVector3 &point, std::vector<GameObject*> objectsInWorld, float radius)
{
	std::vector<std::pair<float, GameObject*>> closeObjects;

	for (auto gO : objectsInWorld)
	{
		if (gO->isSoftBody())
			continue;

		float distance = (reinterpret_cast<RigidBodyObject*>(gO)->Body()->getWorldTransform().getOrigin() - point).length();

		if (distance < radius)
			closeObjects.push_back({ distance, gO });
	}

	float force = 100;

	for (int i = 0; i < closeObjects.size(); ++i)
	{
		GameObject* gO = closeObjects[i].second;
		auto body = reinterpret_cast<RigidBodyObject*>(gO)->Body();

		body->activate(true);
		btVector3 relPos = btVector3(rand() % 200 / 100.0f, rand() % 200 / 100.0f, rand() % 200 / 100.0f);

		btVector3 impulse = body->getWorldTransform().getOrigin() - point;
		impulse.normalize();

		impulse *= force * (1 - (closeObjects[i].first / radius));
		std::cout << (1 - (closeObjects[i].first / radius)) << std::endl;
		body->applyImpulse(impulse, relPos);
	}
	return closeObjects;
}

glm::vec3 HELPERS::ToGLMVec(btVector3* v)
{
	btVector3 vec = *v;
	float x, y, z;
	x = vec.getX();
	y = vec.getY();
	z = vec.getZ();
	return glm::vec3(x, y, z);
}

btRigidBody* HELPERS::addConvexBody(btConvexInternalShape* shape, Shader* shader, btDynamicsWorld* gWorld, float mass, glm::vec3 pos, bool shouldAddToWorld)
{
	btTransform bulletTransform;
	bulletTransform.setIdentity();
	bulletTransform.setOrigin(btVector3(pos));

	btVector3 inertia(0, 0, 0);
	if (mass != 0)
		shape->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(bulletTransform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, shape, inertia);
	btRigidBody* body = new btRigidBody(info);

	if (shouldAddToWorld) gWorld->addRigidBody(body);

	return body;
}

btRigidBody* HELPERS::addTriangleBody(btTriangleMeshShape* shape, Shader* shader, btDynamicsWorld* gWorld, float mass, glm::vec3 pos, bool shouldAddToWorld)
{
	btTransform bulletTransform;
	bulletTransform.setIdentity();
	bulletTransform.setOrigin(btVector3(pos));

	btVector3 inertia(0, 0, 0);

	btMotionState* motion = new btDefaultMotionState(bulletTransform);
	btRigidBody::btRigidBodyConstructionInfo info(0, motion, shape, inertia);
	btRigidBody* body = new btRigidBody(info);

	if (shouldAddToWorld) gWorld->addRigidBody(body);

	return body;
}

btRigidBody* HELPERS::addMeshBody(Shader* shader, btDynamicsWorld* gWorld, Mesh* mesh, glm::vec3 position, float mass, bool shouldAddToWorld)
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

	return addTriangleBody(triangleMesh, shader, gWorld, mass, position, shouldAddToWorld);
}

void HELPERS::addSoftBody(btSoftBody* cloth, Shader* shader, btSoftRigidDynamicsWorld* world, std::vector<GameObject*>& gBodies)
{
	cloth->getCollisionShape()->setMargin(0.001f);
	cloth->generateBendingConstraints(2, cloth->appendMaterial());
	cloth->setTotalMass(3);
	//cloth->m_cfg.citerations = 10;
	//cloth->m_cfg.diterations = 10;
	cloth->m_cfg.viterations = 10;
	cloth->m_cfg.piterations = 10;
	cloth->m_cfg.kDP = 0.005f;
	//cloth->m_cfg.kPR = 10;

	auto gO = new SoftBodyObject(cloth, 1, shader);
	gO->SetColor(glm::vec4(1, 0, 0, 1));
	gBodies.push_back(gO);

	cloth->setUserPointer(gBodies[gBodies.size() - 1]);
	world->addSoftBody(cloth);
}

void HELPERS::CreateCloth(Shader* shader, btSoftRigidDynamicsWorld* world, btSoftBodyWorldInfo* sbwInfo, std::vector<GameObject*>& gBodies, const btScalar s, const int xElements, const int yElements, const int fixed)
{
	btSoftBody* cloth = btSoftBodyHelpers::CreatePatch(*sbwInfo,
		btVector3(-s, s, -s),
		btVector3(s, s, -s),
		btVector3(-s, s, s),
		btVector3(s, s, s),
		xElements, yElements,
		fixed, true);

	btTransform initial;
	initial.setIdentity();
	initial.setOrigin(btVector3(0, 5, 0));

	btQuaternion quat;
	quat.setEuler(90, 0, 0);

	initial.setRotation(quat);
	cloth->transform(initial);

	addSoftBody(cloth, shader, world, gBodies);
}

void HELPERS::CreateClothBall(Shader* shader, btSoftRigidDynamicsWorld* world, btSoftBodyWorldInfo* sbwInfo, std::vector<GameObject*>& gBodies, const btVector3* rad, const btVector3* center, const int res, const int fixed)
{
	btSoftBody* cloth = btSoftBodyHelpers::CreateEllipsoid(*sbwInfo, *center, *rad, res);

	addSoftBody(cloth, shader, world, gBodies);
	cloth->setMass(0, 0);
}


void HELPERS::CreateClothMesh(Shader* shader, btSoftRigidDynamicsWorld* world, btSoftBodyWorldInfo* sbwInfo, std::vector<GameObject*>& gBodies, Mesh* mesh, btVector3* position)
{
	auto meshVerts = mesh->GetVertices();
	auto meshIndices = mesh->GetIndices();
	btScalar* vertices = new btScalar[meshVerts.size() * 3];
	int* indices = new int[meshIndices.size()];

	for (int i = 0; i < meshVerts.size(); ++i)
	{
		auto vertexPos = meshVerts[i].Position();

		vertices[i * 3] = vertexPos.x;
		vertices[i * 3 + 1] = vertexPos.y;
		vertices[i * 3 + 2] = vertexPos.z;
	}

	for (int i = 0; i < meshIndices.size(); ++i)
	{
		indices[i] = meshIndices[i];
	}

	btSoftBody* cloth = btSoftBodyHelpers::CreateFromTriMesh(*sbwInfo, vertices, indices, meshIndices.size() / 3);

	for (int i = 0; i < meshIndices.size() / 3; ++i)
	{
		cloth->m_faces[i].m_n[0]->m_n = btVector3(1, 0, 0); 
	}

	for (int i = 0; i < meshVerts.size(); ++i)
	{
		cloth->m_nodes[i].m_n = btVector3(1, 0, 0);
	}
	cloth->translate(*position);
	addSoftBody(cloth, shader, world, gBodies);
	cloth->setMass(0, 0);

	delete vertices;
	delete indices;
}

void HELPERS::renderSphere(GLUquadricObj* gQuad, GameObject* gO)
{
	auto sphere = reinterpret_cast<RigidBodyObject*>(gO)->Body();
	if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE)
		return;

	glColor3f(1, 0, 0);
	float rad = ((btSphereShape*)sphere->getCollisionShape())->getRadius();

	gluSphere(gQuad, rad, 20, 20);
}

void HELPERS::renderCylinder(GLUquadricObj* gQuad, GameObject* gO)
{
	auto cylinder = reinterpret_cast<RigidBodyObject*>(gO)->Body();
	if (cylinder->getCollisionShape()->getShapeType() != CYLINDER_SHAPE_PROXYTYPE)
		return;

	btVector3 halfExtents = ((btCylinderShape*)cylinder->getCollisionShape())->getHalfExtentsWithoutMargin();

	gluCylinder(gQuad, halfExtents.x(), halfExtents.z(), halfExtents.y() * 2, 20, 20);
}


void HELPERS::renderCone(GLUquadricObj* gQuad, GameObject* gO)
{
	auto cone = reinterpret_cast<RigidBodyObject*>(gO)->Body();
	if (cone->getCollisionShape()->getShapeType() != CONE_SHAPE_PROXYTYPE)
		return;

	float rad = ((btConeShape*)cone->getCollisionShape())->getRadius();
	float height = ((btConeShape*)cone->getCollisionShape())->getHeight();

	gluCylinder(gQuad, rad, 0, height, 20, 20);
}

void HELPERS::renderBox(GLUquadricObj* gQuad, GameObject* gO)
{
	auto box = reinterpret_cast<RigidBodyObject*>(gO)->Body();
	if (box->getCollisionShape()->getShapeType() != BOX_SHAPE_PROXYTYPE)
		return;

	btVector3 halfExtents = ((btCylinderShape*)box->getCollisionShape())->getHalfExtentsWithoutMargin();

	gluCylinder(gQuad, halfExtents.x(), halfExtents.z(), halfExtents.y(), 20, 20);
}

void HELPERS::renderPlane(GLUquadricObj* gQuad, GameObject* gO)
{
	auto plane = reinterpret_cast<RigidBodyObject*>(gO)->Body();
	if (plane->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE)
		return;

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

void HELPERS::renderSoftBody(btSoftBody* softBody, Shader* shader, bool drawDebug)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < softBody->m_faces.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			auto currVert = softBody->m_faces[i].m_n[j]; \

				auto pos = currVert->m_x;
			auto normal = currVert->m_n;
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(pos.x(), pos.y(), pos.z());
		}
	}
	glEnd();

	if (drawDebug)
	{
		shader->SetColor(glm::vec4(0, 0, 0, 1));

		glBegin(GL_LINES);
		for (int i = 0; i < softBody->m_links.size(); ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				auto currVert = softBody->m_links[i].m_n[j];
				glVertex3f(currVert->m_x.x(), currVert->m_x.y(), currVert->m_x.z());
			}
		}
		glEnd();
	}
}

bool HELPERS::RayCastHit(btDynamicsWorld* world, glm::vec3 start, glm::vec3 end)
{
	btVector3 s = start;
	btVector3 e = end;

	btCollisionWorld::ClosestRayResultCallback	closestResults(s, e);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_None;

	world->rayTest(s, e, closestResults);

	if (closestResults.hasHit())
	{
		return true;
	}
	return false;
}

void HELPERS::GetRayCastHit(btDynamicsWorld* world, glm::vec3 start, glm::vec3 end, btVector3& normal, btVector3& position)
{
	btVector3 s = start;
	btVector3 e = end;

	btCollisionWorld::ClosestRayResultCallback	closestResults(s, e);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_None;

	world->rayTest(s, e, closestResults);

	position = closestResults.m_hitPointWorld;//s.lerp(e, closestResults.m_closestHitFraction);
	normal = closestResults.m_hitNormalWorld;
}


GameObject* HELPERS::GetRayCastHitObject(btDynamicsWorld* world, glm::vec3 start, glm::vec3 end, btVector3& normal, btVector3& position)
{
	btVector3 s = start;
	btVector3 e = end;

	btCollisionWorld::ClosestRayResultCallback	closestResults(s, e);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_None;

	world->rayTest(s, e, closestResults);

	position = closestResults.m_hitPointWorld;//s.lerp(e, closestResults.m_closestHitFraction);
	normal = closestResults.m_hitNormalWorld;
	auto hitObj = closestResults.m_collisionObject;
	auto gO = hitObj->getUserPointer();
	return reinterpret_cast<GameObject*>(gO);
}

void HELPERS::DrawCircleAtRayCastHit(btDynamicsWorld* world, glm::vec3 start, glm::vec3 end)
{
	btVector3 s = start;
	btVector3 e = end;

	btCollisionWorld::ClosestRayResultCallback	closestResults(s, e);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_None;

	world->rayTest(s, e, closestResults);

	if (closestResults.hasHit())
	{
		std::cout << "Hit!" << std::endl;
		btVector3 p = s.lerp(e, closestResults.m_closestHitFraction);
	}
}
