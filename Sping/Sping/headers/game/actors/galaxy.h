#pragma once
#ifndef GAME_ACTORS_GALAXY_H
#define GAME_ACTORS_GALAXY_H

#include <octree/octree.h>
#include <boost/uuid/uuid.hpp>
#include <boost/random.hpp>

#include <vector>
#include <memory>

#include <render/renderable.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Game;

struct Star;
struct Galaxy;

struct GalaxyCell
{
	//TODO: each cell may contain a vec of AStar pointers instead of simple points
	std::vector<Star *> stars;
	//may have other things like nebulae etc in the future

	GalaxyCell()
	{
	}
};

//galaxy actor class, comprises of a "galaxy struct" pointer to the galaxy manager's list of galaxies
//for now, i just want one galaxy, but in the (far) future, if you want a whole universe of galaxies, have one universe actor, which has an octree holding all galaxies, and the inception goes deeper...
class AGalaxy
{
private:
	Game *_gamePtr = nullptr;

	boost::uuids::uuid _uuid;
	int64_t _seed;
	boost::mt19937 _module;

	Renderable _renderable; //stuff for meshes etc.

	glm::vec3 _pos;
	glm::vec3 _scale;
	//TODO: seed-based rotation
	glm::mat4 _trans; //transformation within the universe

	Galaxy *_galaxy;

	std::unique_ptr<Octree<GalaxyCell>> _starOctPtr;

	float _octCellSize;
	int _octCellWidthCount;
	float _farthest;

	//TODO: might have multiple "cameras" sending multiple vp's, all being controlled simultanously on multiple framebuffers by a central "player" actor, one for galaxy/universe scale, and one for planet/star scale, layered

public:
	//call the gameptr's galaxygen, grab a galaxy and store it, then run through the stars and dump them in the octree, so next time if the "player" queries "wheres the nearest star from my position" it wont require a nuclear fission reactor to calculate
	void spawn(Game *game, int64_t seed);

	//returns -1 if the star you give has coords that go past the bounds of the octree
	int putStarInOct(Star &star);
	GalaxyCell *getCellInOct(int x, int y, int z);
	GalaxyCell *getCellInOct(const glm::vec3 &coords);
	//not totally correct, but lightweight (compares radiuses instead of the shape of a cube)(will always have some headroom as the "radius" is the farthest "point" of the cube
	bool withinRadius(const glm::vec3 &yourCoords);



	inline boost::uuids::uuid getUUID()
	{
		return this->_uuid;
	}
	
	inline glm::vec3 *getPos()
	{
		return &this->_pos;
	}

	inline glm::mat4 *getTrans()
	{
		return &this->_trans;
	}

protected:
	//haha screw you inheritance
	void createRenderable();
	void partitionStars();

};

#endif