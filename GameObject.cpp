#include "GameObject.h"
#include "Shader.h"
#include "Mesh.h"

#include "Camera.h"


GameObject::GameObject(int i, Shader* shader) : ID(i), m_Hit(false), m_Shader(shader),m_Color(glm::vec4(0,0,1,1)), m_Specular(glm::vec4(1,1,1,20))
{
	
}


GameObject::~GameObject()
{
	//if (m_Shader) delete m_Shader;
}

void GameObject::Draw(GLUquadric* gQuad, Camera* camera)
{
	m_Shader->Bind();
	m_Shader->SetColor(m_Color);
	m_Shader->SetSpecular(m_Specular);
}
