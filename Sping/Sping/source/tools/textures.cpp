#include <tools/textures.h>
#include <SOIL/SOIL.h>
#include <iostream>

Textures::Textures()
{
}

Textures::~Textures()
{
	for (auto element : this->_textures)
		glDeleteTextures(1, &element.second.texID);
}

int Textures::setup()
{
	int ok = this->store("test", "res/textures/test2.jpg", "texture_diffuse");
	if (ok == 0)
	{
		std::cout << "Successfully set up the Texture manager!\n";
		this->list();
		return 0;
	}
	else
	{
		std::cout << "Could not set up initial Texture manager!\n";
		return -1;
	}
}

int Textures::store(std::string name, const char *filePath, const GLchar *type)
{
	if (this->_textures.count(name) != 0)
	{
		std::cout << "Texture with name " << name << " already exists!\n";
		return -1;
	}
	
	int width, height;
	unsigned char *image = SOIL_load_image(filePath, &width, &height, 0, SOIL_LOAD_RGBA);

	Texture tempTex;
	glGenTextures(1, &tempTex.texID);

	glBindTexture(GL_TEXTURE_2D, tempTex.texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	tempTex.type = type;
	this->_textures[name] = tempTex;
	return 0;
}

void Textures::list()
{
	std::cout << "Here's a list of all loaded textures:\n";
	std::map<std::string, Texture>::const_iterator iter;
	iter = this->_textures.begin();
	while (iter != this->_textures.end())
	{
		std::cout << "Texture with name " << iter->first << " and a Tex ID of " << iter->second.texID << " and a type of " << iter->second.type << "\n";
		iter++;
	}
	std::cout << "\n";
}
