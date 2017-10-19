#pragma once
#include "RigidBodyObject.h"
class RigidBodyConvexConeObject :
	public RigidBodyObject
{
public:
	RigidBodyConvexConeObject(int i, Shader* shader, glm::vec3 position, float radius,float height , float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld=true);
	~RigidBodyConvexConeObject();

	void Draw(GLUquadric* gQuad, Camera* camera);
};

