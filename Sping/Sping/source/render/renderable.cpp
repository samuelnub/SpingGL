#include <render/renderable.h>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <game/game.h>

Renderable::Renderable()
{
}

Renderable::~Renderable()
{
}

int Renderable::create(const glm::mat4 spawnPos, 
	Game *game,
	const std::vector<std::string> &shaderNames,
	const std::map<std::string, std::vector<std::string>> &meshAndTexes,
	DrawType drawType,
	DrawPriority drawPriority)
{
	this->_gamePtr = game;
	this->_gamePtr->uuidgen.gen(this->_uuid);

	this->_worldTrans = spawnPos;

	this->_drawType = drawType;
	this->_drawPriority = drawPriority;

	this->_shaderNames = shaderNames;
	this->_meshAndTexes = meshAndTexes;

	this->_shadersVecPtr.clear();
	this->_meshAndTexesVecPtr.clear();



	for (auto &element : this->_shaderNames)
	{
		if (this->_gamePtr->shaders.get(element) == nullptr)
		{
			std::cout << "Couldn't get a shader resource with the name of " << element << " for renderable with UUID " << boost::lexical_cast<std::string>(this->_uuid) << "\n";
			return -1;
		}
		this->_shadersVecPtr.push_back(this->_gamePtr->shaders.get(element));
	}

	for (auto &element : this->_meshAndTexes)
	{
		if (this->_gamePtr->meshes.get(element.first) == nullptr)
		{
			std::cout << "Couldn't get a mesh resource with the name of " << element.first << " for renderable with UUID " << boost::lexical_cast<std::string>(this->_uuid) << "\n";
			return -2;
		}
		for (auto &eleElement : element.second)
		{
			if (this->_gamePtr->textures.get(eleElement) == nullptr)
			{
				std::cout << "Couldn't get a mesh resource with the name of " << eleElement << " for renderable with UUID " << boost::lexical_cast<std::string>(this->_uuid) << "\n";
				return -3;
			}
			this->_meshAndTexesVecPtr[this->_gamePtr->meshes.get(element.first)].push_back(this->_gamePtr->textures.get(eleElement));
		}
	}

	std::cout << "Just created a renderable with a UUID of " << boost::lexical_cast<std::string>(this->_uuid) << "!\n";

	return 0;
}

int Renderable::update(const glm::mat4 newPos,
	const std::vector<std::string> &shaderNames,
	const std::map<std::string, std::vector<std::string>> &meshAndTexes,
	DrawType drawtype,
	DrawPriority drawPriority)
{
	this->_worldTrans = newPos;

	this->_drawType = drawtype;
	this->_drawPriority = drawPriority;

	this->_shaderNames = shaderNames;
	this->_meshAndTexes = meshAndTexes;

	this->_shadersVecPtr.clear();
	this->_meshAndTexesVecPtr.clear();

	for (auto &element : this->_shaderNames)
	{
		if (this->_gamePtr->shaders.get(element) == nullptr)
		{
			std::cout << "Couldn't get a shader resource with the name of " << element << " for renderable with UUID " << boost::lexical_cast<std::string>(this->_uuid) << "\n";
			return -1;
		}
		this->_shadersVecPtr.push_back(this->_gamePtr->shaders.get(element));
	}

	for (auto &element : this->_meshAndTexes)
	{
		if (this->_gamePtr->meshes.get(element.first) == nullptr)
		{
			std::cout << "Couldn't get a mesh resource with the name of " << element.first << " for renderable with UUID " << boost::lexical_cast<std::string>(this->_uuid) << "\n";
			return -2;
		}
		for (auto &eleElement : element.second)
		{
			if (this->_gamePtr->textures.get(eleElement) == nullptr)
			{
				std::cout << "Couldn't get a mesh resource with the name of " << eleElement << " for renderable with UUID " << boost::lexical_cast<std::string>(this->_uuid) << "\n";
				return -3;
			}
			this->_meshAndTexesVecPtr[this->_gamePtr->meshes.get(element.first)].push_back(this->_gamePtr->textures.get(eleElement));
		}
	}

	return 0;
}

void Renderable::info()
{

}
