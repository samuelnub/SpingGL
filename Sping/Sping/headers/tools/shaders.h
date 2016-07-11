#pragma once
#ifndef TOOLS_SHADER_H
#define TOOLS_SHADER_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <SDL2/SDL_opengl.h>
#include <map>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

struct Shader
{
	GLuint programID;
	//shader specific uniforms you may need to send, like ssao dithering quality etc.
	std::map<const GLchar *, GLfloat> uniformFloats;
	std::map<const GLchar *, glm::vec3> uniformVec3s;
	std::map<const GLchar *, glm::mat4> uniformMat4s;

	/*friend bool operator< (const Shader &shaderOne, const Shader &shaderTwo)
	{
		return shaderOne.programID < shaderTwo.programID;
	}*/

};

//shader manager
class Shaders
{
private:
	std::map<std::string, Shader> _shaders;

public:
	Shaders();
	~Shaders();

	int setup();

	//compiles and stores it in this manager
	int compile(std::string name, const GLchar *vertPath, const GLchar *fragPath);

	void utilise(const Shader *shader);

	inline Shader *get(std::string name)
	{
		try
		{
			return &this->_shaders.at(name);
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