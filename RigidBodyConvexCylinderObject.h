#pragma once
#include "RigidBodyObject.h"
class RigidBodyConvexCylinderObject :
	public RigidBodyObject
{
public:
	RigidBodyConvexCylinderObject(int i, Shader* shader, glm::vec3 position, float radius,float height , float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld=true);
	~RigidBodyConvexCylinderObject();

	void Draw(GLUquadric* gQuad, Camera* camera);
};

