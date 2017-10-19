#pragma once
#include "Transform.h"
#include <BulletSoftBody/btSoftBody.h>
class Camera;
class Shader;
class Mesh;
class btRigidBody;
class GLUquadric;

class GameObject
{
public:
	GameObject(int i, Shader* shader);
	virtual ~GameObject();

	bool isSoftBody() const { return m_IsSoftBody; }
	const std::string &Tag() const { return m_Tag; }


	const int ID = -1;
	void SetTag(std::string tag) { m_Tag = tag; }

	virtual void Draw(GLUquadric* gQuad, Camera* camera);

	void SetSpecularColor(glm::vec3 color)
	{
		m_Specular.x = color.x;
		m_Specular.y = color.y;
		m_Specular.z = color.z;
	}

	void SetSpecularColor(float shininess)
	{
		m_Specular.a = shininess;
	}

	glm::vec4 GetSpecular() const { return m_Specular; }

	void SetColor(glm::vec4 color) { m_Color = color; }
	void SetAlpha(float alpha) { m_Color.a = alpha; }

protected:
	GameObject(const GameObject& other) {}
	GameObject& operator=(const GameObject& other) {}

	bool m_IsSoftBody = false;

	Shader* m_Shader;
	glm::vec4 m_Color;
	glm::vec4 m_Specular;

	std::string m_Tag;
	bool m_Hit;
	Transform m_Transform;
};

