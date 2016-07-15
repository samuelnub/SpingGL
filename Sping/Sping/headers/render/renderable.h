#pragma once
#ifndef RENDER_RENDERABLE_H
#define RENDER_RENDERABLE_H

#include <vector>
#include <map>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Game;

struct Mesh;
struct Shader;
struct Texture;

//note: do not inherit off of this, instead, compose with this
//an instance of a renderable only contains necessary information such as transformation in world space, and pointers to meshes, textures and shaders held in the scene object. Instances of these renderables should exist only in the scene's Renderable map, with strings denotating each instance. tldr its like that "model" class from before, but with shared resources
class Renderable
{
private:
	uint64_t _ID;

	glm::mat4 _worldTrans;

	Game *_gamePtr;

	std::vector<std::string> _shaderNames;
	std::map<std::string, std::vector<std::string>> _meshAndTexes;

	std::vector<Shader *> _shadersVecPtr;
	std::map<Mesh *, std::vector<Texture *>> _meshAndTexesVecPtr;

public:
	Renderable();
	~Renderable();

	//specify initial resources to use, your upper level "actors" which involve this class will set up this in their own constructor
	int create(const int64_t id, const glm::mat4 spawnPos,
		Game *game,
		const std::vector<std::string> &shaderNames,
		const std::map<std::string, std::vector<std::string>> &meshAndTexes);

	//called each frame, just updates pos
	inline void update(const glm::mat4 newPos)
	{
		this->_worldTrans = newPos;
	}

	//overridden if you need to update the assets it refers to
	int update(const glm::mat4 newPos,
		const std::vector<std::string> &shaderNames,
		const std::map<std::string, std::vector<std::string>> &meshAndTexes);
	
	//inline functions cant be forward declared :(((
	inline int64_t getID()
	{
		this->_ID;
	}

	inline glm::mat4 *getTrans() 
	{
		return &this->_worldTrans;
	}

	inline std::vector<std::string> *getShaderNames()
	{
		if (this->_shaderNames.size() == 0)
		{
			return nullptr;
		}
		return &this->_shaderNames;
	}

	inline std::map<std::string, std::vector<std::string>> *getMeshTexNames()
	{
		if (this->_meshAndTexes.size() == 0)
		{
			return nullptr;
		}
		return &this->_meshAndTexes;
	}

	inline std::vector<Shader *> *getShaderPtrs()
	{
		if (this->_shadersVecPtr.size() == 0)
		{
			return nullptr;
		}
		return &this->_shadersVecPtr;
	}

	inline std::map<Mesh *, std::vector<Texture *>> *getMeshTexPtrs()
	{
		if (this->_meshAndTexesVecPtr.size() == 0)
		{
			return nullptr;
		}
		return &this->_meshAndTexesVecPtr;
	}

	//lists info about its own instance, meshes it uses etc
	void info(); 

protected:
	

};

#endif