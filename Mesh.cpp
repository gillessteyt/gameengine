#include "Mesh.h"
#include <vector>
#include "tiny_obj_loader.h"

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	InitModel model;

	for (int i = 0; i < numVertices; ++i)
	{
		m_Vertices.push_back(vertices[i]);
		model.positions.push_back(vertices[i].Position());
		model.texCoords.push_back(vertices[i].TexCoord());
	}

	for (int i = 0; i < numIndices; ++i)
	{
		m_Indices.push_back(indices[i]);
		model.indices.push_back(indices[i]);
	}

	InitMesh(model);
}

Mesh::Mesh(const std::string& fileName)
{
	InitModel model;
	tinyobj::attrib_t attrib;
	
	std::vector<tinyobj::shape_t> shapes;
	
	std::vector<tinyobj::material_t> materials;

	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), nullptr, true);
	auto vertices = attrib.vertices;
	auto normals = attrib.normals;
	auto texCoords = attrib.vertices;


	std::vector<glm::vec3> positionsVec;
	std::vector<glm::vec3> normalsVec;
	std::vector<glm::vec2> texCoordsVec;

	auto indexVec = shapes[0].mesh.indices;

	for (int i = 0; i < vertices.size(); i+=3)
	{
		positionsVec.push_back(glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
		normalsVec.push_back(glm::vec3(normals[i], normals[i + 1], normals[i + 2]));
	}

	for (int i = 0; i < vertices.size(); i += 3)
	{
		texCoordsVec.push_back(glm::vec2(texCoords[i], texCoords[i + 1]));
	}

	for (int i = 0; i < vertices.size()/3; ++i)
	{
		m_Vertices.push_back(Vertex(positionsVec[i],texCoordsVec[i],normalsVec[i]));
		model.positions.push_back(positionsVec[i]);
		model.normals.push_back(normalsVec[i]);
		model.texCoords.push_back(texCoordsVec[i]);
	}

	for (int i = 0; i < indexVec.size(); ++i)
	{
		m_Indices.push_back(indexVec[i].vertex_index);
		model.indices.push_back(indexVec[i].vertex_index);
	}

	InitMesh(model);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VertexArrayObject);
}

void Mesh::Draw()
{
	glBindVertexArray(m_VertexArrayObject);

	glDrawElements(GL_TRIANGLES, m_DrawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::InitMesh(const InitModel& model)
{
	m_DrawCount = model.indices.size();

	glGenVertexArrays(1, &m_VertexArrayObject);
	glBindVertexArray(m_VertexArrayObject);


	glGenBuffers(NUM_BUFFERS, m_VertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);


	glBindBuffer(GL_ARRAY_BUFFER, m_VertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	if (!model.normals.empty())
	{
		glGenBuffers(NUM_BUFFERS, m_VertexArrayBuffers);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexArrayBuffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}
