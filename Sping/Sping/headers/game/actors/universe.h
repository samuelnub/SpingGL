#pragma once
#ifndef GAME_ACTORS_UNIVERSE_H
#define GAME_ACTORS_UNIVERSE_H

#include <octree/octree.h>
#include <boost/uuid/uuid.hpp>
#include <boost/random.hpp>

#include <vector>
#include <memory>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Game;

class AGalaxy;

struct UniverseCell
{
	std::vector<AGalaxy *> galaxies;

	UniverseCell()
	{
	}
};

//'aight, the universe? now you're just playing god
//there shouldnt be a renderable aspect to this, unless you want to visualise the octree
//actor manager may have a unique ptr to hold an instance of this
//https://www.youtube.com/watch?v=WG3Sl3m4rNs pretty good resource for spatial partitioning and a tad bit on unique_ptr
class AUniverse
{
private:
	Game *_gamePtr = nullptr;

	boost::uuids::uuid _uuid;
	int64_t _seed;
	boost::mt19937 _module;

	std::unique_ptr<Octree<UniverseCell>> _galaxyOctPtr;
	
	float _octCellSize; 
	int _octCellWidthCount;

public:
	void spawn(Game *game, int64_t seed);

	UniverseCell *getCellInOct(int x, int y, int z);
	UniverseCell *getCellInOct(const glm::vec3 &coords);

	inline boost::uuids::uuid getUUID()
	{
		return this->_uuid;
	}

protected:
	void spawnGalaxy();

};

#endif                                                                                                                                                      