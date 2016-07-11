#include <render/renderable.h>
#include <iostream>

#include <tools/meshes.h>
#include <tools/shaders.h>
#include <tools/textures.h>

Renderable::Renderable()
{
}

Renderable::~Renderable()
{
}

int Renderable::create(const int64_t id, const glm::mat4 spawnPos, 
	Meshes *meshesSrc,
	Shaders *shadersSrc,
	Textures *texturesSrc,
	const std::vector<std::string> &shaderNames,
	const std::map<std::string, std::vector<std::string>> &meshAndTexes)
{
	this->_ID = id;

	this->_worldTrans = spawnPos;

	this->_shaderNames = shaderNames;
	this->_meshAndTexes = meshAndTexes;

	this->_shadersVecPtr.clear();
	this->_meshAndTexesVecPtr.clear();

	//i forgot to initialise these, and kept getting access violation errors at runtime, nice one dumbo
	this->_meshesPtr = meshesSrc;
	this->_shadersPtr = shadersSrc;
	this->_texturesPtr = texturesSrc;

	//TODO: check for nullptrs
	for (auto &element : this->_shaderNames)
	{
		this->_shadersVecPtr.push_back(this->_shadersPtr->get(element));
	}

	for (auto &element : this->_meshAndTexes)
	{
		for (auto &eleElement : element.second)
		{
			this->_meshAndTexesVecPtr[this->_meshesPtr->get(element.first)].push_back(this->_texturesPtr->get(eleElement));
		}
	}

	std::cout << "Just created a renderable with an ID of " << this->_ID << "!\n";

	return 0;
}

int Renderable::update(const glm::mat4 newPos,
	const std::vector<std::string> &shaderNames,
	const std::map<std::string, std::vector<std::string>> &meshAndTexes)
{
	this->_worldTrans = newPos;

	this->_shaderNames = shaderNames;
	this->_meshAndTexes = meshAndTexes;

	this->_shadersVecPtr.clear();
	this->_meshAndTexesVecPtr.clear();

	for (auto &element : this->_shaderNames)
	{
		this->_shadersVecPtr.push_back(this->_shadersPtr->get(element));
	}

	for (auto &element : this->_meshAndTexes)
	{
		for (auto &eleElement : element.second)
		{
			this->_meshAndTexesVecPtr[this->_meshesPtr->get(element.first)].push_back(this->_texturesPtr->get(eleElement));
		}
	}

	return 0;
}

void Renderable::info()
{

}
