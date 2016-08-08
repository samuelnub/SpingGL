#include <game/actors/galaxy.h>
#include <game/game.h>

#include <iostream>
#include <string>

#include <boost/random/uniform_real_distribution.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

void AGalaxy::spawn(Game * game, int64_t seed)
{
	this->_gamePtr = game;
	this->_gamePtr->uuidgen.gen(this->_uuid);
	this->_seed = seed;
	this->_module.seed(seed);

	std::cout << "Spawning a galaxy with a UUID of " << boost::lexical_cast<std::string>(this->_uuid) << " and a given seed of " << this->_seed << "!\n";
	
	this->_gamePtr->actormanager.galaxygen.generate(this->_seed);
	this->_galaxy = this->_gamePtr->actormanager.galaxygen.get(this->_seed);
	
	//put every star in the octree for easy querying
	this->partitionStars();
	
	double posBounds =
		(this->_gamePtr->settings.getGraphicsSettings().universeCellSize *
			this->_gamePtr->settings.getGraphicsSettings().universeCellWidthCount) / 2;
	boost::random::uniform_real_distribution<> posDistrib(-posBounds, posBounds);

	this->_pos.x = posDistrib(this->_module);
	this->_pos.y = posDistrib(this->_module);
	this->_pos.z = posDistrib(this->_module);
	
	//limit bounds to make sure the galaxy isnt scaled past the size of the cell width
	boost::random::uniform_real_distribution<> scaleDistrib(this->_gamePtr->settings.getGraphicsSettings().universeCellSize * 0.1f, this->_gamePtr->settings.getGraphicsSettings().universeCellSize * 0.9f);
	double scale = scaleDistrib(this->_module);
	this->_scale = glm::vec3(scale, scale, scale);

	this->_trans = glm::translate(this->_pos);
	this->_trans = glm::scale(this->_scale);
	

	this->createRenderable();
}

void AGalaxy::createRenderable()
{
	std::cout << "Creating renderable for said galaxy...\n";

	//naming convention for storing mesh data for galaxy actors:
	std::string storeName = "AGalaxy-" + boost::lexical_cast<std::string>(this->_uuid);

	std::vector<Vertex> tempVertices;
	std::vector<GLuint> tempIndices;

	for (unsigned i = 0; i < this->_galaxy->stars.size(); i++)
	{
		tempVertices.push_back(this->_galaxy->stars.at(i).vertInfo);
		tempIndices.push_back(i++);
	}

	this->_gamePtr->meshes.store(storeName, tempVertices, tempIndices);
	//TODO: load proper shaders for desired actors who need them during main setup of program
	std::vector<std::string> tempShaderNames;
	tempShaderNames.emplace_back("test");

	std::map<std::string, std::vector<std::string>> tempMeshTexNames;
	tempMeshTexNames[storeName].push_back(storeName);

	this->_renderable.create(this->_trans, this->_gamePtr, tempShaderNames, tempMeshTexNames, DrawType::POINTS);
}

void AGalaxy::partitionStars()
{
	//based on vec3 pos, where to place in a predefined-size octree? get the farthest star and make an aabb (well not really a tightly bound bounding box, just a box that manages to encompass all stars)

	glm::vec3 farthestStarPos = glm::vec3();
	//fun fact, the words "further" and "farther" are often utilised interchangably by a very large majority of the population; but in reality, they slightly differ in meaning. the former has a formal definition of referring to distances which are figurative and purely allegorical, whereas the latter is used to represent physical and tangible distances, such as by means of space or time. surprisingly, although these two words only appear to differentiate by a single letter, they both have separate timelines from which they evolved from - with "further" inheriting traits from the olden english and germanic words "furthor", "furthra" and "fyrthrian" during the midpoint of the middle ages, and also borrowing a slight resemblance from the modern english word "forth" as in "to come forth and commit thine work" - not to be confused with "fourth", which is the element denoted by the numeric digit 3 in a standard container such as an array. by contrast, the word "farther" simply stems off the word "far" as we can easily recognise in our modern-day english language, which in turn stemmed from a git merge of the olden english words of "feorr"; its dutch translation - "ver", as well as minor contributions from latin word "porro", which meant "forward", also during the late first millennium; during the 900's, a time period where the capability of :q! was not yet widely known by vim users

	//this operation doesnt act on transformed galaxies, just purely generated; original data, whereby you can scale and transform the octree to your hearts desire
	for (auto &element : this->_galaxy->stars)
	{
		if (glm::length(element.vertInfo.pos) > glm::length(farthestStarPos))
		{
			farthestStarPos = element.vertInfo.pos;
		}
	}

	//make it positive if its negative
	farthestStarPos.x = (farthestStarPos.x < 0.0f) ? farthestStarPos.x * -1 : farthestStarPos.x;
	farthestStarPos.y = (farthestStarPos.y < 0.0f) ? farthestStarPos.y * -1 : farthestStarPos.y;
	farthestStarPos.z = (farthestStarPos.z < 0.0f) ? farthestStarPos.z * -1 : farthestStarPos.z;

	//get the farthest among the three axies
	this->_farthest = (farthestStarPos.x > farthestStarPos.y) ? farthestStarPos.x : farthestStarPos.y;
	this->_farthest = (this->_farthest > farthestStarPos.z) ? this->_farthest : farthestStarPos.z;

	this->_octCellWidthCount = this->_gamePtr->settings.getGraphicsSettings().galaxyCellWidthCount;

	this->_octCellSize = (this->_octCellWidthCount / 2) / this->_farthest;

	this->_starOctPtr = std::make_unique<Octree<GalaxyCell>>(this->_octCellWidthCount, GalaxyCell());

	//index based on original star vert pos data, because we're assuming these stars will be static
	for (auto &element : this->_galaxy->stars)
	{
		this->putStarInOct(element);
	}
}

int AGalaxy::putStarInOct(Star &star)
{
	if (glm::length(star.vertInfo.pos) >
		glm::length(glm::vec3(this->_farthest, this->_farthest, this->_farthest)))
	{
		return -1;
	}

	//cant call getCellInOct(pos) because itll return you a nullptr if the node doesnt exist, which we want to create in this case lol
	glm::vec3 properPos;

	properPos.x = (star.vertInfo.pos.x < 0.0f) ? star.vertInfo.pos.x * -1 : star.vertInfo.pos.x;
	properPos.y = (star.vertInfo.pos.y < 0.0f) ? star.vertInfo.pos.y * -1 : star.vertInfo.pos.y;
	properPos.z = (star.vertInfo.pos.z < 0.0f) ? star.vertInfo.pos.z * -1 : star.vertInfo.pos.z;

	properPos *= 2;

	int cellX = static_cast<int>(properPos.x / this->_octCellSize);
	int cellY = static_cast<int>(properPos.y / this->_octCellSize);
	int cellZ = static_cast<int>(properPos.z / this->_octCellSize);

	this->_starOctPtr->operator()(cellX, cellY, cellZ).stars.push_back(&star);

	return 0;
}

GalaxyCell *AGalaxy::getCellInOct(int x, int y, int z)
{
	if (x > this->_octCellWidthCount ||
		y > this->_octCellWidthCount ||
		z > this->_octCellWidthCount)
	{
		return nullptr;
	}

	if (this->_starOctPtr->at(x, y, z).stars.empty())
	{
		return nullptr;
	}
	else
	{
		return &this->_starOctPtr->operator()(x, y, z);
	}
}

GalaxyCell *AGalaxy::getCellInOct(const glm::vec3 &coords)
{
	if (glm::length(coords) >
		glm::length(glm::vec3(this->_farthest, this->_farthest, this->_farthest)))
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

	if (this->_starOctPtr->at(cellX, cellY, cellZ).stars.empty())
	{
		return nullptr;
	}
	else
	{
		return &this->_starOctPtr->operator()(cellX, cellY, cellZ);
	}
}

bool AGalaxy::withinRadius(const glm::vec3 & yourCoords)
{
	if (glm::length(yourCoords) <= glm::length(glm::vec3(this->_farthest, this->_farthest, this->_farthest)))
	{
		return true;
	}
	return false;
}
