#include <tools/generators/galaxygen.h>
#include <iostream>

Elliptical::Elliptical()
{
}

Elliptical::~Elliptical()
{
}

void Elliptical::generate(std::mt19937 module,
	float size, float densityMean, float densityDeviation, float deviationX, float deviationY, float deviationZ)
{
	this->_size = size;

	this->_densityMean = densityMean;
	this->_densityDeviation = densityDeviation;

	this->_deviationX = deviationX;
	this->_deviationY = deviationY;
	this->_deviationZ = deviationZ;

	std::cout << "Making vertex data for an elliptical galaxy...\n";

	std::normal_distribution<> normDistrib(this->_densityMean, this->_densityDeviation);
	double density = std::fmax(0.0f, normDistrib(module));

	std::cout << density << " is the density\n";

	unsigned int countMax = std::fmax(0, static_cast<int>(this->_size * this->_size * this->_size * density));

	std::cout << countMax << " is the count\n";

	if (countMax <= 0)
	{
		return;
	}

	std::uniform_int_distribution<> uniDistrib(1, countMax);
	int count = uniDistrib(module);

	std::normal_distribution<> tempNDx(0, this->_deviationX * this->_size);
	std::normal_distribution<> tempNDy(0, this->_deviationY * this->_size);
	std::normal_distribution<> tempNDz(0, this->_deviationZ * this->_size);


	glm::vec3 pos;
	Vertex tempVert;
	tempVert.norm = glm::vec3(0.0f, 0.0f, 0.0f);
	tempVert.uv = glm::vec2(0.0f, 0.0f);

	for (int i = 0; i < count; i++)
	{
		pos.x = tempNDx(module);
		pos.y = tempNDy(module);
		pos.z = tempNDz(module);

		//wanna make it 100x faster? then dont output to console you dumbo
		//std::cout << pos.x << ", " << pos.y << ", " << pos.z << "//\n";

		tempVert.pos = pos;

		this->_vertexData.push_back(tempVert);
		this->_indexData.push_back(i++);
	}
	std::cout << "Successfully generated " << count << " stars.\n";
}

Irregular::Irregular()
{
}

Irregular::~Irregular()
{
}

void Irregular::generate(std::mt19937 module, float metaSize, float countMean, float countDeviation, float deviationX, float deviationY, float deviationZ)
{
	std::cout << "Generating an irregular galaxy based off ellipticals...\n";

	this->_countMean = countMean;
	this->_countDeviation = countDeviation;

	this->_metaSize = metaSize;

	this->_deviationX = deviationX;
	this->_deviationY = deviationY;
	this->_deviationZ = deviationZ;

	std::normal_distribution<> normDistrib(this->_countMean, this->_countDeviation);
	double count = std::fmax(0, normDistrib(module));

	std::cout << "Count is " << count << "\n";

	if (count <= 0)
	{
		return;
	}

	count = 5;

	std::normal_distribution<> tempNDx(0, this->_deviationX);
	std::normal_distribution<> tempNDy(0, this->_deviationY);
	std::normal_distribution<> tempNDz(0, this->_deviationZ);

	glm::vec3 centerPos;

	GLuint indexCount = 0;

	for (unsigned int i = 0; i < count; i++)
	{
		centerPos.x = tempNDx(module);
		centerPos.y = tempNDy(module);
		centerPos.z = tempNDz(module);

		std::cout << centerPos.x << " " << centerPos.y << " " << centerPos.z << "\n";

		Elliptical ellipTemp;
		ellipTemp.generate(module, this->_metaSize);
		std::vector<Vertex> tempVerts;
		tempVerts = ellipTemp.getVerts();

		for (unsigned int j = 0; j < tempVerts.size(); j++)
		{
			tempVerts.at(j).pos += centerPos;
			this->_vertexData.push_back(tempVerts.at(j));
			this->_indexData.push_back(indexCount++);
			indexCount++;
		}

	}
	std::cout << "Successfully generated " << indexCount << " stars in total\n";
}

GalaxyGenerator::GalaxyGenerator()
{
}

GalaxyGenerator::~GalaxyGenerator()
{
}

int GalaxyGenerator::setup()
{
	std::cout << "Successfully set up the galaxy generator!\n";
	return 0;
}

int GalaxyGenerator::generate(int64_t seed)
{
	std::mt19937 module;
	module.seed(seed);
	std::uniform_int_distribution<> galaxyType(0, 2);
	int choice = galaxyType(module);

	switch (choice)
	{
	case static_cast<int>(GalaxyType::ELLIPTICAL) :
		this->genElliptical(seed);
		break;
	case static_cast<int>(GalaxyType::IRREGULAR) :
		this->genIrregular(seed);
		break;
	case static_cast<int>(GalaxyType::SPIRAL) :
		this->genSpiral(seed);
		break;
	default:
		std::cout << "Uh, something went wrong with trying to generate a galaxy\n";
		return -1;
	}

	return 0;
}
