#pragma once
#include "GameObject.h"
#include <BulletSoftBody/btSoftBody.h>

class SoftBodyObject :
	public GameObject
{
public:
	SoftBodyObject(btSoftBody* body, int i, Shader* shader);
	~SoftBodyObject();

	virtual void Draw(GLUquadric* gQuad, Camera* camera);

	btSoftBody* Body() const { return m_Body; }

private:
	btSoftBody* m_Body;
};

