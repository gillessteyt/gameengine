#pragma once
#include <glm/glm.hpp>
#include <glew/glew.h>
#include <string>
#include <vector>

struct InitModel
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
}; 

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texC, const glm::vec3& normal = glm::vec3(0,0,0)) : m_Position(pos), m_TexCoord(texC), m_Normal(normal)
	{

	}
	glm::vec3& Position() { return m_Position; }
	glm::vec3& Normal() { return m_Normal; }
	glm::vec2& TexCoord() { return m_TexCoord; }
protected:
private:
	glm::vec3 m_Position;
	glm::vec2 m_TexCoord;
	glm::vec3 m_Normal;
};

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Mesh(const std::string &fileName);
	
	~Mesh();
	const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
	const std::vector<unsigned int>& GetIndices() const { return m_Indices; }
	void Draw();
private:
	Mesh(const Mesh& other) {}
	Mesh& operator=(const Mesh& other) {}

	void InitMesh(const InitModel& model);

	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		INDEX_VB,
		NORMAL_VB,
		NUM_BUFFERS
	};

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	GLuint m_VertexArrayObject;
	GLuint m_VertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_DrawCount;
};

