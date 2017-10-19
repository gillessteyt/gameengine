#include "Shader.h"
#include "Texture.h"
#include <fstream>
#include <iostream>
#include "Camera.h"
#include <GL/gl.h>

static std::string LoadShader(const std::string& fileName);
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName)
{
	m_Program = glCreateProgram();
	m_Shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_Shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; ++i)
		glAttachShader(m_Program, m_Shaders[i]);

	glBindAttribLocation(m_Program, 0, "position");
	glBindAttribLocation(m_Program, 1, "texCoord");
	glBindAttribLocation(m_Program, 2, "normal");

	glLinkProgram(m_Program);
	CheckShaderError(m_Program, GL_LINK_STATUS, true, "Error: Program linking failed in Shader: ");

	glValidateProgram(m_Program);
	CheckShaderError(m_Program, GL_VALIDATE_STATUS, true, "Error: Program is invalid in Shader: ");

	m_Uniforms[TRANSFORM_U] = glGetUniformLocation(m_Program, "viewProj");
	m_Uniforms[WORLD_U] = glGetUniformLocation(m_Program, "world");
	m_Uniforms[COLOR_U] = glGetUniformLocation(m_Program, "color");
	m_Uniforms[SPECULAR_U] = glGetUniformLocation(m_Program, "specular");
	m_Uniforms[EYEPOS_U] = glGetUniformLocation(m_Program, "eyePos");
	
	m_Uniforms[USEDIFFUSEMAP_U] = glGetUniformLocation(m_Program, "useDiffuse");
	m_Uniforms[DIFFUSETEX_U] = glGetUniformLocation(m_Program, "samDiffuse");

	m_Uniforms[USEOPACITYMAP_U] = glGetUniformLocation(m_Program, "useOpacity");
	m_Uniforms[OPACITYTEX_U] = glGetUniformLocation(m_Program, "samOpacity");

	glUniform1i(m_Uniforms[USEDIFFUSEMAP_U], false);
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; ++i)
	{
		glDetachShader(m_Program, m_Shaders[i]);
		glDeleteShader(m_Shaders[i]);
	}

	glDeleteProgram(m_Program);
}

void Shader::Bind()
{
	glUseProgram(m_Program);
}

void Shader::Update(const Transform& transform, const Camera* camera)
{
	glm::mat4 worldPerspectiveMat = camera->GetViewProjection() * transform.GetWorldMatrix();

	glUniformMatrix4fv(m_Uniforms[TRANSFORM_U], 1, GL_FALSE, &worldPerspectiveMat[0][0]);
	glUniformMatrix4fv(m_Uniforms[WORLD_U], 1, GL_FALSE, &transform.GetWorldMatrix()[0][0]);

	glm::vec4 eyePos = glm::vec4(camera->GetPosition(), 1);
	glUniform4fv(m_Uniforms[EYEPOS_U], 1, reinterpret_cast<GLfloat*>(&eyePos));

	glUniform1i(m_Uniforms[USEDIFFUSEMAP_U], m_UseDiffuseTexture);
	if (m_UseDiffuseTexture)
		m_DiffuseTexture->Bind(0);

	glUniform1i(m_Uniforms[USEOPACITYMAP_U], m_UseOpacityTexture);
	if (m_UseOpacityTexture)
		m_OpacityTexture->Bind(1);
}

void Shader::SetColor(glm::vec4 color)
{
	glUniform4fv(m_Uniforms[COLOR_U], 1, reinterpret_cast<GLfloat*>(&color));
}

void Shader::SetSpecular(glm::vec4 color)
{
	glUniform4fv(m_Uniforms[SPECULAR_U], 1, reinterpret_cast<GLfloat*>(&color));
}

void Shader::SetDiffuseTexture(Texture* texture)
{
	m_UseDiffuseTexture = true;
	glUniform1i(m_Uniforms[USEDIFFUSEMAP_U], true);
	m_DiffuseTexture = texture;

	glUniform1i(m_Uniforms[DIFFUSETEX_U], 0);
}

void Shader::SetOpacityTexture(Texture* texture)
{
	m_UseOpacityTexture = true;
	glUniform1i(m_Uniforms[USEOPACITYMAP_U], true);
	m_OpacityTexture = texture;

	glUniform1i(m_Uniforms[OPACITYTEX_U], 0);
}

static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: Shader Creation failed!" << std::endl;

	const GLchar* shaderSourceStrings = text.c_str();
	GLint shaderSourceStringLengths = text.length();

	glShaderSource(shader, 1, &shaderSourceStrings, &shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader failed to compile: ");
	return shader;
}

static std::string LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName.c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
		std::cerr << "Unable to load shader: " << fileName << std::endl;

	return output;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}