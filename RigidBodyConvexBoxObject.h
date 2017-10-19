#pragma once
#include "RigidBodyObject.h"
class RigidBodyConvexBoxObject :
	public RigidBodyObject
{
public:
	RigidBodyConvexBoxObject(int i, Shader* shader, glm::vec3 position, glm::vec3 extents, Mesh* meshBox, float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld=true);
	~RigidBodyConvexBoxObject();

	void Draw(GLUquadric* gQuad, Camera* camera);

	void SetMesh(Mesh* mesh) { m_Mesh = mesh; }

private:
	Mesh* m_Mesh;
};

