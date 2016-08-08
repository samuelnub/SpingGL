#include <game/actors/universe.h>
#include <game/game.h>
#include <tools/generators/uuidgen.h>

#include <iostream>

#include <boost/random.hpp>

#include <game/actors/galaxy.h>

void AUniverse::spawn(Game * game, int64_t seed)
{
	std::cout << "Spawning a universe...\n";

	this->_gamePtr = game;
	this->_gamePtr->uuidgen.gen(this->_uuid);
	this->_seed = seed;

	this->_octCellSize = this->_gamePtr->settings.getGraphicsSettings().universeCellSize;
	this->_octCellWidthCount = this->_gamePtr->settings.getGraphicsSettings().universeCellWidthCount;
	
	this->_galaxyOctPtr = std::make_unique<Octree<UniverseCell>>(this->_octCellWidthCount, UniverseCell());

	this->_module.seed(seed);

	for (unsigned int i = 0; i < this->_gamePtr->settings.getGraphicsSettings().galaxyMaxCount; i++)
	{
		this->spawnGalaxy();
	}
}

UniverseCell * AUniverse::getCellInOct(int x, int y, int z)
{
	if (x > this->_octCellWidthCount ||
		y > this->_octCellWidthCount ||
		z > this->_octCellWidthCount)
	{
		return nullptr;
	}

	if (this->_galaxyOctPtr->at(x, y, z).galaxies.empty())
	{
		return nullptr;
	}
	else
	{
		return &this->_galaxyOctPtr->operator()(x, y, z);
	}
}

UniverseCell * AUniverse::getCellInOct(const glm::vec3 & coords)
{
	if (glm::length(coords) >
		glm::length(glm::vec3(
			this->_octCellSize * (this->_octCellWidthCount / 2), 
			this->_octCellSize * (this->_octCellWidthCount / 2),
			this->_octCellSize * (this->_octCellWidthCount / 2))))
	{
		return nullptr;
	}

	glm::vec3 properPos;

	properPos.x = (coords.x < 0.0f) ? coords.x * -1 : coords.x;
	properPos.y = (coords.y < 0.0f) ? coords.y * -1 : coords.y;
	properPos.z = (coords.z < 0.0f) ? coords.z * -1 : coords.z;

	properPos *= 2;

	int cellX = static_cast<int>(properPos.x / this->_octCellSize);
	int cellY = static_cast<int>(properPos.y / this->_octCellSize);
	int cellZ = static_cast<int>(properPos.z / this->_octCellSize);

	if (this->_galaxyOctPtr->at(cellX, cellY, cellZ).galaxies.empty())
	{
		return nullptr;
	}
	else
	{
		return &this->_galaxyOctPtr->operator()(cellX, cellY, cellZ);
	}
}

void AUniverse::spawnGalaxy()
{
	//galaxy actors spawned will obey the bounds of the universe set in graphics settings
	this->_gamePtr->actormanager.galaxies.emplace_back(AGalaxy());
	this->_gamePtr->actormanager.galaxies.back().spawn(this->_gamePtr, this->_module());
	
	//get pos of spawned galaxy... put it in octree.at(based on cell size)
	glm::vec3 spawnedGalPos = *this->_gamePtr->actormanager.galaxies.back().getPos();

	glm::vec3 properPos;

	properPos.x = (spawnedGalPos.x < 0.0f) ? spawnedGalPos.x * -1 : spawnedGalPos.x;
	properPos.y = (spawnedGalPos.y < 0.0f) ? spawnedGalPos.y * -1 : spawnedGalPos.y;
	properPos.z = (spawnedGalPos.z < 0.0f) ? spawnedGalPos.z * -1 : spawnedGalPos.z;

	properPos *= 2;

	int cellX = static_cast<int>(properPos.x / this->_octCellSize);
	int cellY = static_cast<int>(properPos.y / this->_octCellSize);
	int cellZ = static_cast<int>(properPos.z / this->_octCellSize);

	this->_galaxyOctPtr->operator()(cellX, cellY, cellZ).galaxies.push_back(&this->_gamePtr->actormanager.galaxies.back());


}
