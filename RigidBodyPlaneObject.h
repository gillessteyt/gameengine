#pragma once
#include "RigidBodyObject.h"

class RigidBodyPlaneObject :
	public RigidBodyObject
{
public:
	RigidBodyPlaneObject(int i, Shader* shader, glm::vec3 position, btDynamicsWorld* gWorld, bool shouldAddToWorld = true);
	~RigidBodyPlaneObject();

	void Draw(GLUquadric* gQuad, Camera* camera);
};

