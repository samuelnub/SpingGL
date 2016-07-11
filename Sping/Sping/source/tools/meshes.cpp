#include <tools/meshes.h>
#include <iostream>

Meshes::Meshes()
{
}

Meshes::~Meshes()
{
	for (auto element : this->_meshes)
		glDeleteVertexArrays(1, &element.second.vaoID);
	for (auto element : this->_meshes)
		glDeleteBuffers(1, &element.second.vboID);
	for (auto element : this->_meshes)
		glDeleteBuffers(1, &element.second.eboID);
}

int Meshes::setup()
{
	this->_pushCount = 0;

	std::vector<Vertex> quad1 =
	{
		{
			glm::vec3(0.5f, 0.5f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f)
		},
		{
			glm::vec3(0.5f, -0.5f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec2(1.0f, 0.0f)
		},
		{
			glm::vec3(-0.5f, -0.5f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f)
		},
		{
			glm::vec3(-0.5f, 0.5f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f)
		}
	};

	std::vector<GLuint> quad1indices =
	{
		0, 1, 3,
		1, 2, 3
	};


	int ok = this->store("test", quad1, quad1indices);
	if (ok == 0)
	{
		std::cout << "Successfully set up the Mesh manager!\n";
		this->list();
		return 0;
	}
	else
	{
		std::cout << "Something went horribly wrong with setting up the mesh manager!\n";
		return -1;
	}
}

int Meshes::store(const std::string name, std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
	if (this->_meshes.count(name) != 0)
	{
		std::cout << "Mesh " << name << " already exists!\n";
		return -1;
	}

	//push count is like the unique ID, by the way, it stores it in the vector starting from 1, not 0 indexing, so vertexData[0] should theoretically be empty
	//TODO: also read this limit from user config
	if (this->_pushCount >= 10000 - 2)
	{
		std::cout << "Reached mesh limit!\a\n";
		return -2;
	}

	this->_pushCount++;


	this->_meshes[name].pushID = this->_pushCount;

	this->_vertexData.at(this->_meshes[name].pushID) = vertices;
	this->_indexData.at(this->_meshes[name].pushID) = indices;

	glGenVertexArrays(1, &this->_meshes[name].vaoID);
	glGenBuffers(1, &this->_meshes[name].vboID);
	glGenBuffers(1, &this->_meshes[name].eboID);

	glBindVertexArray(this->_meshes[name].vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, this->_meshes[name].vboID);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(Vertex),
		&this->_vertexData.at(this->_meshes[name].pushID),
		GL_STATIC_DRAW
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_meshes[name].eboID);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(GLuint),
		&this->_indexData.at(this->_meshes[name].pushID),
		GL_STATIC_DRAW
		);

	//Vertex attributes

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)0
		);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, norm)
		);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, uv)
		);

	glBindVertexArray(0);

	return 0;
}

void Meshes::list()
{
	std::cout << "Here are all your currently loaded meshes:\n";
	std::map<std::string, Mesh>::const_iterator iter;
	iter = this->_meshes.begin();
	while (iter != this->_meshes.end())
	{
		std::cout << "Mesh called " << iter->first << " with a pushID of " << iter->second.pushID << " with a VBO ID of " << iter->second.vboID << ", a VAO ID of " << iter->second.vaoID << " and an EBO ID of " << iter->second.eboID << " and an index count of " << this->_indexData[iter->second.pushID].size() << "\n";
		iter++;
	}
	std::cout << "\n";
}
