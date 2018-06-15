#include "stdafx.h"
#include "BoundingBox.h"

unsigned int boundingboxIndices[] = {
	0, 1, 1, 2, 2, 3, 3, 0,
	1, 5, 2, 6, 3, 7, 0, 4,
	4, 5, 5, 6, 6, 7, 7, 4
};

BoundingBox::BoundingBox()
	: m_Init(false)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
}


BoundingBox::~BoundingBox()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void BoundingBox::Render(Shader shader)
{
	if (!m_Init){
		initialize();
		m_Init = true;
	}
	shader.use();
	glBindVertexArray(m_VAO);
	glDrawElements(GL_LINES, sizeof(boundingboxIndices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

AABB& BoundingBox::GetAABB()
{
	return m_AABB;
}

void BoundingBox::SetAABB(const AABB& aabb)
{
	m_AABB = aabb;
}

void BoundingBox::initialize()
{
	glm::vec3 aabbMin = m_AABB.getMin();
	glm::vec3 aabbMax = m_AABB.getMax();
	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3(aabbMin.x, aabbMax.y, aabbMin.z));//left top back
	positions.push_back(glm::vec3(aabbMin.x, aabbMax.y, aabbMax.z));//left top front
	positions.push_back(glm::vec3(aabbMax.x, aabbMax.y, aabbMax.z));//right top front
	positions.push_back(glm::vec3(aabbMax.x, aabbMax.y, aabbMin.z));//right top back
	positions.push_back(glm::vec3(aabbMin.x, aabbMin.y, aabbMin.z));//left bottom back
	positions.push_back(glm::vec3(aabbMin.x, aabbMin.y, aabbMax.z));//left bottom front
	positions.push_back(glm::vec3(aabbMax.x, aabbMin.y, aabbMax.z));//right bottom front
	positions.push_back(glm::vec3(aabbMax.x, aabbMin.y, aabbMin.z));//right bottom back

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*positions.size(), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boundingboxIndices), boundingboxIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}
