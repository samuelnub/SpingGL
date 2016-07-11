#pragma once
#ifndef TOOLS_MESH_H
#define TOOLS_MESH_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <SDL2/SDL_opengl.h>
#include <vector>
#include <array>
#include <map>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 uv;
};

struct Mesh
{
	GLuint vboID;
	GLuint eboID;
	GLuint vaoID; 
	GLuint pushID;
};

//mesh manager, permanant storage of meshes for duration of program
class Meshes
{
private:
	std::map<std::string, Mesh> _meshes;

	//TODO: read this limit off of user config, i'd call this one'a those so-called "engine limitations"
	std::array<std::vector<Vertex>, 10000> _vertexData;
	std::array<std::vector<GLuint>, 10000> _indexData;

	GLuint _pushCount;

public:
	Meshes();
	~Meshes();

	int setup();

	//give me pure vertex and index data and i'll make a vbo, ebo and vao
	int store(const std::string name, std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);

	inline Mesh *get(std::string name)
	{
		try
		{
			return &this->_meshes.at(name);
		}
		catch (std::exception err)
		{
			return nullptr;
		}
	}

	inline std::vector<Vertex> *getVertData(GLuint index)
	{
		return &this->_vertexData.at(index);
	}

	inline std::vector<GLuint> *getIndexData(GLuint index)
	{
		return &this->_indexData.at(index);
	}

	//list all mesh names youve stored, and their buffer/attrib id's
	void list();

protected:


};

#endif