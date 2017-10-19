#pragma once
#include "RigidBodyObject.h"
class RigidBodyTriangleMeshObject :
	public RigidBodyObject
{
public:
	RigidBodyTriangleMeshObject(int i, Shader* shader, glm::vec3 position, Mesh* mesh, float mass, btDynamicsWorld* gWorld, bool shouldAddToWorld=true);
	~RigidBodyTriangleMeshObject();

	void SetMesh(Mesh* mesh) { m_Mesh = mesh; }

	void Draw(GLUquadric* gQuad, Camera* camera);

private:
	Mesh* m_Mesh;

};

