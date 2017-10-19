#pragma once
#include "RigidBodyObject.h"
class RigidBodyConvexSphereObject :
	public RigidBodyObject
{
public:
	RigidBodyConvexSphereObject(int i, Shader* shader, glm::vec3 position, float radius, float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld=true);
	~RigidBodyConvexSphereObject();

	void Draw(GLUquadric* gQuad, Camera* camera);
};

