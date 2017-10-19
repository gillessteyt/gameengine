#pragma once
#include <string>
#include <glew/glew.h>

class Texture
{
public:
	Texture(const std::string &fileName);
	virtual ~Texture();

	void Bind(unsigned int unit);
private:
	Texture(const Texture& other) {}
	Texture& operator=(const Texture& other) {}

	GLuint m_Texture;
};

