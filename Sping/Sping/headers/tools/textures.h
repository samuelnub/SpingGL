#pragma once
#ifndef TOOLS_TEXTURE_H
#define TOOLS_TEXTURE_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <SDL2/SDL_opengl.h>

#include <SOIL/SOIL.h>
#include <map>
#include <string>

//naming convention: make sure your shader has a "material" struct with sampler2D members called "texture_diffuse1"... or just "texture_" + type + number, check scene.cpp's draw definition to see what uniform name it sends.
struct Texture
{
	GLuint texID;
	const GLchar *type; //provide the name "diffuse" "specular" or "normal" (TODO)
};

//texture manager
class Textures
{
private:
	std::map<std::string, Texture> _textures;

public:
	Textures();
	~Textures();

	int setup();

	int store(std::string name, const char *filePath, const GLchar *type);

	inline Texture *get(std::string name)
	{
		try
		{
			return &this->_textures.at(name);
		}
		catch (std::exception err)
		{
			return nullptr;
		}
	}

	void list();

protected:


};

#endif