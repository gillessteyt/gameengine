#pragma once
#include <string>
#include <glew/glew.h>
#include "Transform.h"
class Texture;
class Camera;
class Shader
{
public:
	Shader(const std::string& fileName);

	virtual ~Shader();

	void Bind();
	void Update(const Transform& transform, const Camera* camera);

	void SetColor(glm::vec4 color);
	void SetSpecular(glm::vec4 color);
	void SetDiffuseTexture(Texture* texture);
	void SetOpacityTexture(Texture* texture);
private:
	Shader(const Shader& other) {}
	Shader& operator=(const Shader& other) {}

	static const unsigned int NUM_SHADERS = 3;
	Texture* m_DiffuseTexture = nullptr;
	bool m_UseDiffuseTexture = false;

	Texture* m_OpacityTexture = nullptr;
	bool m_UseOpacityTexture = false;
	enum
	{
		TRANSFORM_U,
		WORLD_U,
		COLOR_U,
		SPECULAR_U,
		EYEPOS_U,
		USEDIFFUSEMAP_U,
		DIFFUSETEX_U,
		USEOPACITYMAP_U,
		OPACITYTEX_U,
		NUM_UNIFORMS
	};

	GLuint m_Program;
	GLuint m_Shaders[NUM_SHADERS];
	GLuint m_Uniforms[NUM_UNIFORMS];
};

