#pragma once
#include <btBulletDynamicsCommon.h>

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include <vector>
#include <glew/glew.h>
#include "GameObject.h"

namespace HELPERS
{
	std::vector<std::pair<float, GameObject*>> GetObjectsInRadiusFromPoint(btVector3 &point, std::vector<GameObject*> objectsInWorld, float radius);

	glm::vec3 ToGLMVec(btVector3* v);

	btRigidBody* addConvexBody(btConvexInternalShape* shape, Shader* shader, btDynamicsWorld* gWorld, float mass, glm::vec3 pos, bool shouldAddToWorld);

	btRigidBody* addTriangleBody(btTriangleMeshShape* shape, Shader* shader, btDynamicsWorld* gWorld, float mass, glm::vec3 pos, bool shouldAddToWorld);
	
	btRigidBody* addMeshBody(Shader* shader, btDynamicsWorld* gWorld, Mesh* mesh, glm::vec3 position, float mass, bool shouldAddToWorld = true);

	void addSoftBody(btSoftBody* cloth, Shader* shader, btSoftRigidDynamicsWorld* world, std::vector<GameObject*>& gBodies);

	void CreateCloth(Shader* shader, btSoftRigidDynamicsWorld* world, btSoftBodyWorldInfo* sbwInfo, std::vector<GameObject*>& gBodies, const btScalar s, const int xElements, const int yElements, const int fixed = 4 + 8);

	void CreateClothBall(Shader* shader, btSoftRigidDynamicsWorld* world, btSoftBodyWorldInfo* sbwInfo, std::vector<GameObject*>& gBodies, const btVector3* rad, const btVector3* center, const int res, const int fixed = 4 + 8);


	void CreateClothMesh(Shader* shader, btSoftRigidDynamicsWorld* world, btSoftBodyWorldInfo* sbwInfo, std::vector<GameObject*>& gBodies, Mesh* mesh, btVector3* position);

	void renderSphere(GLUquadricObj* gQuad, GameObject* gO);

	void renderCylinder(GLUquadricObj* gQuad, GameObject* gO);

	void renderCone(GLUquadricObj* gQuad, GameObject* gO);

	void renderBox(GLUquadricObj* gQuad, GameObject* gO);

	void renderPlane(GLUquadricObj* gQuad, GameObject* gO);

	void renderSoftBody(btSoftBody* softBody, Shader* shader, bool drawDebug = false);

	bool RayCastHit(btDynamicsWorld* world, glm::vec3 start, glm::vec3 end);

	void GetRayCastHit(btDynamicsWorld* world, glm::vec3 start, glm::vec3 end, btVector3& normal, btVector3& position);


	GameObject* GetRayCastHitObject(btDynamicsWorld* world, glm::vec3 start, glm::vec3 end, btVector3& normal, btVector3& position);

	void DrawCircleAtRayCastHit(btDynamicsWorld* world, glm::vec3 start, glm::vec3 end);
}