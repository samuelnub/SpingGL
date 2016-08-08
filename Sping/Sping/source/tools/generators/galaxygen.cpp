#include <tools/generators/galaxygen.h>
#include <game/game.h>

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/math/special_functions/round.hpp>

#include <cmath>
#include <climits>

#include <iostream>

const double PI = 3.14159265f;

GalaxyGenerator::GalaxyGenerator()
{
}

GalaxyGenerator::~GalaxyGenerator()
{
}

int GalaxyGenerator::setup(Game *game)
{
	this->_gamePtr = game;

	std::cout << "Successfully set up the galaxy generator!\n";
	return 0;
}

Galaxy GalaxyGenerator::generate(int64_t seed)
{
	//if (this->_galaxies.at(seed).seed == seed)
	{
		//return this->_galaxies.at(seed);
	}

	boost::mt19937 module(seed);
	boost::random::uniform_int_distribution<> uniDistrib(0, 2); //the 3 options
	int choice = uniDistrib(module);

	Galaxy tempGalaxy;
	tempGalaxy.seed = seed;
	this->_gamePtr->uuidgen.gen(tempGalaxy.uuid);
	choice = 2; //TODO: may just make it spiral-only
	switch (choice)
	{
	case static_cast<int>(GalaxyType::ELLIPTICAL) :
		std::cout << "Generating an Elliptical galaxy with a UUID of " << boost::lexical_cast<std::string>(tempGalaxy.uuid) << "...\n";
		tempGalaxy.type = GalaxyType::ELLIPTICAL;
		tempGalaxy.stars = this->genElliptical(module);
		break;

	case static_cast<int>(GalaxyType::IRREGULAR) :
		std::cout << "Generating an Irregular galaxy with a UUID of " << boost::lexical_cast<std::string>(tempGalaxy.uuid) << "...\n";
		tempGalaxy.type = GalaxyType::IRREGULAR;
		tempGalaxy.stars = this->genIrregular(module);
		break;

	case static_cast<int>(GalaxyType::SPIRAL) :
		std::cout << "Generating a Spiral galaxy with a UUID of " << boost::lexical_cast<std::string>(tempGalaxy.uuid) << "...\n";
		tempGalaxy.type = GalaxyType::SPIRAL;
		tempGalaxy.stars = this->genSpiral(module);
		break;

	default:
		std::cout << "Uh, something went wrong with trying to generate a galaxy\n";
		break;
	}

	//just in case its somehow empty, we dont wanna give the mesh class an empty vec
	Star centerStar;
	centerStar.seed = seed; //hehe, give it the same seed given to this galaxy
	this->_gamePtr->uuidgen.gen(centerStar.uuid);
	centerStar.vertInfo.pos = glm::vec3(0.0f, 0.0f, 0.0f);
	this->color(centerStar, module, StarType::BLACK);
	tempGalaxy.stars.push_back(centerStar);

	std::cout << "Successfully generated " << tempGalaxy.stars.size() << " stars.\n";
	this->_galaxies[seed] = tempGalaxy;
	return tempGalaxy;
}

std::vector<Star> GalaxyGenerator::genElliptical(boost::mt19937 &module, bool shouldBeSmall)
{
	double sizeMax;
	if (shouldBeSmall == true)
	{
		sizeMax = 1200.0f;
	}
	else if (shouldBeSmall == false)
	{
		sizeMax = 30000.0f;
	}

	boost::random::normal_distribution<> sizeDistrib(sizeMax, sizeMax/1000.0f);
	double size = sizeDistrib(module);
	
	boost::random::uniform_real_distribution<> densityMeanDistrib(0.0002f, 0.0003f);
	double densityMean = densityMeanDistrib(module);
	
	boost::random::uniform_real_distribution<> densityDeviationDistrib(0.0000005f, 0.0000015f);
	double densityDeviation = densityDeviationDistrib(module);
	
	boost::normal_distribution<> densityDistrib(densityMean, densityDeviation);
	double density = densityDistrib(module);

	unsigned int countMax = static_cast<int>(size * size * density);
	if (countMax <= 0)
	{
		std::cout << "Uh, countMax was <= 0\n";
	}

	boost::random::uniform_int_distribution<> countDistrib(countMax * 0.75, countMax);
	unsigned int count = countDistrib(module);

	boost::random::uniform_real_distribution<> deviationXDistrib(0.000001f, 0.0000035f);
	boost::random::uniform_real_distribution<> deviationYDistrib(0.000001f, 0.0000035f);
	boost::random::uniform_real_distribution<> deviationZDistrib(0.000001f, 0.0000035f);
	
	boost::random::normal_distribution<> deviationX(0, deviationXDistrib(module) * size);
	boost::random::normal_distribution<> deviationY(0, deviationYDistrib(module) * size);
	boost::random::normal_distribution<> deviationZ(0, deviationZDistrib(module) * size);

	boost::random::uniform_int_distribution<int64_t> starSeedDistrib(0, LLONG_MAX);

	std::vector<Star> tempStars;
	glm::vec3 tempPos;
	for (unsigned int i = 0; i < count; i++)
	{
		Star tempStar;
		tempStar.seed = starSeedDistrib(module);

		tempPos.x = deviationX(module);
		tempPos.y = deviationY(module);
		tempPos.z = deviationZ(module);
		
		this->color(tempStar, module);
		this->_gamePtr->uuidgen.gen(tempStar.uuid);

		tempStar.vertInfo.pos = tempPos;
		tempStars.push_back(tempStar);
	}
	return tempStars;
}

std::vector<Star> GalaxyGenerator::genIrregular(boost::mt19937 &module)
{
	boost::random::uniform_int_distribution<> countDistrib(200, 500);
	unsigned int count = countDistrib(module);

	boost::random::uniform_real_distribution<> deviationXDistrib(0.00001f, 0.00004f);
	boost::random::uniform_real_distribution<> deviationYDistrib(0.00001f, 0.00004f);
	boost::random::uniform_real_distribution<> deviationZDistrib(0.00001f, 0.00004f);

	boost::random::normal_distribution<> deviationX(0, deviationXDistrib(module));
	boost::random::normal_distribution<> deviationY(0, deviationYDistrib(module));
	boost::random::normal_distribution<> deviationZ(0, deviationZDistrib(module));

	std::vector<Star> tempStars;
	glm::vec3 offset;
	for (unsigned int i = 0; i < count; i++)
	{
		offset.x = deviationX(module)*10;
		offset.y = deviationY(module)*10;
		offset.z = deviationZ(module)*10;

		std::vector<Star> tempMetaStars;
		tempMetaStars = this->genElliptical(module, true);

		for (auto &element : tempMetaStars)
		{
			this->offset(element, offset);
		}
		tempStars.insert(std::end(tempStars), std::begin(tempMetaStars), std::end(tempMetaStars));
	}
	return tempStars;
}

std::vector<Star> GalaxyGenerator::genSpiral(boost::mt19937 &module)
{
	boost::random::normal_distribution<> swirlDistrib(5.0f, 1.0f);
	double swirl = PI * swirlDistrib(module);

	boost::random::normal_distribution<> swirlOffsetDistrib(swirl, 0.04f);

	boost::random::uniform_int_distribution<> sizeDistrib(1000, 10000);
	unsigned int size = sizeDistrib(module);
	
	//make a cluster and scale it down to make it dense, like me
	boost::random::uniform_real_distribution<> centerScaleDistrib(0.2f, 0.3f);
	double centerScale = centerScaleDistrib(module) * (size*0.0008f);

	boost::random::uniform_real_distribution<> armScaleDistrib(0.1f, 0.25f);

	std::vector<Star> tempStars;
	glm::vec3 upVec = glm::vec3(0.0f, 1.0f, 0.0f);
	
	//generate arms================================================
	boost::random::uniform_int_distribution<> armCountDistrib(2, 7);
	unsigned int armCount = armCountDistrib(module);
	double armAngle = (PI * 2) / armCount;
	boost::random::uniform_real_distribution<> backgroundLengthDistrib(static_cast<float>(armCount), 8.0f);
	boost::random::uniform_int_distribution<> spacingDistrib(2, 5);
	unsigned int spacing = spacingDistrib(module);
	unsigned int maxMetas = (size / spacing) / armCount;
	boost::random::uniform_real_distribution<> metaCenterDistrib(0.15f, 0.25f);
	boost::random::uniform_real_distribution<> metaCountDistrib(maxMetas * 0.8f, maxMetas * 1.8f);

	boost::random::uniform_int_distribution<> schemeDistrib(0, 5); //dont want a black hole scheme lol
	boost::random::normal_distribution<> schemeArmDistrib(schemeDistrib(module), 0.75f);

	double metaCenter = metaCenterDistrib(module);
	for (int arm = 0; arm < armCount; arm++)
	{
		unsigned int metaCount = boost::math::round(metaCountDistrib(module));

		for (int i = 0; i < metaCount; i++)
		{
			//angle from center of arm
			boost::random::normal_distribution<> angleDistrib(0, 0.3f * armAngle * metaCenter);
			float angle = angleDistrib(module) + armAngle * arm;

			//distance along this particular arm
			boost::normal_distribution<> distDistrib(0, size * 0.000004f);
			float distance = std::abs(distDistrib(module));

			glm::vec3 center = glm::vec3(0.0f, 0.0f, distance);
			glm::quat q = glm::angleAxis(angle, upVec);

			StarType schemeArm = static_cast<StarType>(boost::math::iround(schemeArmDistrib(module)));

			center = q * center;
			std::vector<Star> tempArmMetaStars;
			tempArmMetaStars = this->genElliptical(module, true);
			double metaScale = armScaleDistrib(module);
			for (auto &element : tempArmMetaStars)
			{
				this->scale(element, glm::vec3(metaScale, metaScale, metaScale));
				this->offset(element, center);
				this->swirl(element, upVec, swirlOffsetDistrib(module), module);
				this->color(element, module, schemeArm);
			}
			tempStars.insert(std::end(tempStars), std::begin(tempArmMetaStars), std::end(tempArmMetaStars));
		}
	}
	
	//generate center densely-packed cluster (or irregular)==========
	StarType schemeCenter = static_cast<StarType>(schemeDistrib(module));
	std::vector<Star> tempCenterStars;
	tempCenterStars = this->genIrregular(module);
	for (auto &element : tempCenterStars)
	{
		this->scale(element, glm::vec3(centerScale, centerScale * 0.6f, centerScale));
		this->swirl(element, upVec, swirlOffsetDistrib(module) * 5, module);
		this->color(element, module, schemeCenter);
	}
	tempStars.insert(std::end(tempStars), std::begin(tempCenterStars), std::end(tempCenterStars));
	tempCenterStars = this->genIrregular(module);
	for (auto &element : tempCenterStars)
	{
		this->scale(element, glm::vec3(centerScale * 4.0f, centerScale * 0.6f, centerScale));
		this->swirl(element, upVec, swirl * 5, module);
		this->color(element, module, schemeCenter);
	}
	tempStars.insert(std::end(tempStars), std::begin(tempCenterStars), std::end(tempCenterStars));

	//generate background stars=====================================
	std::vector<Star> tempBackgroundStars;
	tempBackgroundStars = this->genIrregular(module);
	for (auto &element : tempBackgroundStars)
	{
		this->scale(element, glm::vec3(centerScale, centerScale * 0.4f, centerScale * backgroundLengthDistrib(module)));
		this->swirl(element, upVec, swirlOffsetDistrib(module), module);
	}
	tempStars.insert(std::end(tempStars), std::begin(tempBackgroundStars), std::end(tempBackgroundStars));
	tempBackgroundStars = this->genIrregular(module);
	for (auto &element : tempBackgroundStars)
	{
		this->scale(element, glm::vec3(centerScale * backgroundLengthDistrib(module) * 0.5f, centerScale * 0.3f, centerScale * backgroundLengthDistrib(module)));
		this->swirl(element, upVec, swirlOffsetDistrib(module), module);
	}
	tempStars.insert(std::end(tempStars), std::begin(tempBackgroundStars), std::end(tempBackgroundStars));
	tempBackgroundStars = this->genElliptical(module, true);
	for (auto &element : tempBackgroundStars)
	{
		this->scale(element, glm::vec3(centerScale * backgroundLengthDistrib(module), centerScale * 0.4f, centerScale * backgroundLengthDistrib(module) * 0.75f));
		this->swirl(element, upVec, swirlOffsetDistrib(module), module);
	}
	tempStars.insert(std::end(tempStars), std::begin(tempBackgroundStars), std::end(tempBackgroundStars));

	return tempStars;
}

void GalaxyGenerator::offset(Star & star, const glm::vec3 & offset)
{
	star.vertInfo.pos += offset;
}

void GalaxyGenerator::scale(Star & star, const glm::vec3 & scale)
{
	star.vertInfo.pos *= scale;
}

void GalaxyGenerator::swirl(Star & star, const glm::vec3 & axis, float amount, boost::mt19937 &module)
{
	//TODO: messing around to get cool swirl effects only lol
	boost::random::uniform_real_distribution<> upDownMulDistrib(0.8f, 2.4f);
	star.vertInfo.pos.y *= upDownMulDistrib(module);

	boost::random::uniform_int_distribution<> extendTempYDistrib(4, 40);
	glm::vec3 tempPos = star.vertInfo.pos;
	tempPos.y *= extendTempYDistrib(module);
	//HAHAH I WAS DOING DIST = STAR.VERTINFO.POS.LENGTH() WHICH JUST GAVE ME THE SIZE OF THE VEC3 WHICH IS 3 GG
	double dist = glm::length(tempPos);

	float ang = static_cast<float>(std::pow(dist, 0.1f) * amount);
	//std::cout << ang << " is the angle apparently\n";
	glm::quat q = glm::angleAxis(ang, axis);
	//std::cout << star.vertInfo.pos.x << " " << star.vertInfo.pos.y << " " << star.vertInfo.pos.z << " before\n";
	q = glm::rotate(q, ang, axis);
	star.vertInfo.pos = q * star.vertInfo.pos;
	//std::cout << star.vertInfo.pos.x << " " << star.vertInfo.pos.y << " " << star.vertInfo.pos.z << " after\n\n";
}

void GalaxyGenerator::color(Star & star, boost::mt19937 &module, StarType scheme)
{
	boost::random::uniform_int_distribution<> starTypeDistrib(0, 10); //dangling numbers will use default: which will be the scheme
	boost::random::uniform_int_distribution<> starLuminosityDistrib(0, 4);

	int color = starTypeDistrib(module);
	int luminosity = starLuminosityDistrib(module);
	//TODO: color based on proximity to 0,0,0, outwards getting dimmer/fading from blues to yellow/orange
	//reference res/textures/starColor.bmp for these "strange UV coords"
	switch (color)
	{
	case static_cast<int>(StarType::WHITE) :
		star.type = StarType::WHITE;
		star.vertInfo.uv.x = static_cast<float>(1 / 14.0f);
		break;
	case static_cast<int>(StarType::BLUE) :
		star.type = StarType::BLUE;
		star.vertInfo.uv.x = static_cast<float>(3 / 14.0f);
		break;
	case static_cast<int>(StarType::YELLOW):
		star.type = StarType::YELLOW;
		star.vertInfo.uv.x = static_cast<float>(5 / 14.0f);
		break;
	case static_cast<int>(StarType::ORANGE) :
		star.type = StarType::ORANGE;
		star.vertInfo.uv.x = static_cast<float>(7 / 14.0f);
		break;
	case static_cast<int>(StarType::RED) :
		star.type = StarType::RED;
		star.vertInfo.uv.x = static_cast<float>(9 / 14.0f);
		break;
	case static_cast<int>(StarType::BROWN) :
		star.type = StarType::BROWN;
		star.vertInfo.uv.x = static_cast<float>(11 / 14.0f);
		break;
	case static_cast<int>(StarType::BLACK) :
		star.type = StarType::BLACK;
		star.vertInfo.uv.x = static_cast<float>(13 / 14.0f);
		break;
	default:
		star.type = scheme;
		star.vertInfo.uv.x = static_cast<float>((static_cast<float>(scheme)*2+1) / 14.0f);
		break; // just in case lol, just make it yellow
	}

	switch (luminosity)
	{
	case static_cast<int>(StarLuminosity::BRIGHTEST) :
		star.luminosity = StarLuminosity::BRIGHTEST;
		star.vertInfo.uv.y = static_cast<float>(1 / 10.0f);
		break;
	case static_cast<int>(StarLuminosity::BRIGHT) :
		star.luminosity = StarLuminosity::BRIGHT;
		star.vertInfo.uv.y = static_cast<float>(3 / 10.0f);
		break;
	case static_cast<int>(StarLuminosity::AVERAGE) :
		star.luminosity = StarLuminosity::AVERAGE;
		star.vertInfo.uv.y = static_cast<float>(5 / 10.0f);
		break;
	case static_cast<int>(StarLuminosity::DIM) :
		star.luminosity = StarLuminosity::DIM;
		star.vertInfo.uv.y = static_cast<float>(7 / 10.0f);
		break;
	case static_cast<int>(StarLuminosity::DIMMEST) :
		star.luminosity = StarLuminosity::DIMMEST;
		star.vertInfo.uv.y = static_cast<float>(9 / 10.0f);
		break;
	default:
		star.luminosity = StarLuminosity::AVERAGE;
		star.vertInfo.uv.y = static_cast<float>(5 / 10.0f);
		break;
	}
}
