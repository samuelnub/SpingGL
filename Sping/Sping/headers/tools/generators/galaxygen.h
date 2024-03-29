#pragma once
#ifndef TOOLS_GENERATORS_GALAXYGEN_H
#define TOOLS_GENERATORS_GALAXYGEN_H

#include <boost/uuid/uuid.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/lexical_cast.hpp>

#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tools/meshes.h>

//credit due to https://github.com/martindevans/CasualGodComplex
class Game;

enum class GalaxyType
{
	ELLIPTICAL,
	IRREGULAR,
	SPIRAL,

	ENUM_SIZE
};

enum class StarType
{
	WHITE,
	BLUE,
	YELLOW,
	ORANGE,
	RED,
	BROWN,
	BLACK //wow, black after white? racist scum
};

enum class StarLuminosity
{
	BRIGHTEST,
	BRIGHT,
	AVERAGE,
	DIM,
	DIMMEST //me
};

struct Star //a star point's data, whereby a star actor can expand upon it
{
	boost::uuids::uuid uuid; //the star actor will copy this uuid
	int64_t seed; //probably going to be the seed for the star actor too
	StarType type; //size determined later, with seed, since for now its just a colored point
	StarLuminosity luminosity;
	Vertex vertInfo;
};

struct Galaxy
{
	boost::uuids::uuid uuid;
	int64_t seed; //the seed it was given
	GalaxyType type;
	std::vector<Star> stars;
};

//galaxy holder, not a galaxy actor holder, that's the actor manager's job.
//the galaxy actor should compose of a galaxy that was generated by this single class
class GalaxyGenerator 
{
private:
	Game *_gamePtr = nullptr;

	std::unordered_map<int64_t, Galaxy> _galaxies;

public:
	GalaxyGenerator();
	~GalaxyGenerator();

	int setup(Game *game);

	//seed determines the type
	Galaxy generate(int64_t seed);

	inline Galaxy *get(int64_t ID)
	{
		try
		{
			return &this->_galaxies.at(ID);
		}
		catch (std::exception err)
		{
			return nullptr;
		}
	}

protected:
	//elliptical is the basis for the others, so theres a special "should be small" if the others are using it
	std::vector<Star> genElliptical(boost::mt19937 &module, bool shouldBeSmall = false);
	std::vector<Star> genIrregular(boost::mt19937 &module);
	std::vector<Star> genSpiral(boost::mt19937 &module);

	void offset(Star &star, const glm::vec3 &offset);
	void scale(Star &star, const glm::vec3 &scale);
	void swirl(Star &star, const glm::vec3 &axis, float amount, boost::mt19937 &module);
	void color(Star &star, boost::mt19937 &module, StarType scheme = StarType::RED); //based on type, apply UV coords
};

#endif