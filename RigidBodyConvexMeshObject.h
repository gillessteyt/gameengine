#pragma once
#include "RigidBodyObject.h"
class RigidBodyConvexMeshObject :
	public RigidBodyObject
{
public:
	RigidBodyConvexMeshObject(int i, Shader* shader, glm::vec3 position, Mesh* mesh, float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld=true);
	~RigidBodyConvexMeshObject();

	void Draw(GLUquadric* gQuad, Camera* camera);

	void SetMesh(Mesh* mesh) { m_Mesh = mesh; }

private:
	Mesh* m_Mesh;
};

