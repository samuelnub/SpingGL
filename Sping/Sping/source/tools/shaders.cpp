#include <tools/shaders.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shaders::Shaders()
{
}

Shaders::~Shaders()
{
	for (auto element : this->_shaders)
		glDeleteProgram(element.second.programID);
}

int Shaders::setup()
{
	int ok = this->compile("test", "res/shaders/testVert.glsl", "res/shaders/testFrag.glsl");
	if (ok != 0)
	{
		std::cout << "Uh, couldn't setup the initial test shader, you might be screwed\n";
		return -1;
	}
	else
	{
		std::cout << "Successfully set up the shader manager!\n";
		this->list();
		return 0;
	}
}

int Shaders::compile(std::string name, const GLchar * vertPath, const GLchar * fragPath)
{
	if (this->_shaders.count(name) != 0)
	{
		std::cout << "Shader with name " << name << " already exists!\n";
		return -1;
	}

	std::string vertCode;
	std::string fragCode;
	std::ifstream vertFile;
	std::ifstream fragFile;

	vertFile.exceptions(std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::badbit);

	try
	{
		vertFile.open(vertPath);
		fragFile.open(fragPath);

		std::stringstream vertStream;
		std::stringstream fragStream;

		vertStream << vertFile.rdbuf();
		fragStream << fragFile.rdbuf();

		vertFile.close();
		fragFile.close();

		vertCode = vertStream.str();
		fragCode = fragStream.str();
	}
	catch (std::ifstream::failure err)
	{
		std::cout << "Could not read shader file " << vertPath << " or " << fragPath << "!\n";
		return -1;
	}

	const GLchar *vertShaderCode = vertCode.c_str();
	const GLchar *fragShaderCode = fragCode.c_str();

	GLuint vert, frag;
	GLint succ;
	GLchar log[512];

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertShaderCode, NULL);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &succ);
	if(!succ)
	{
		glGetShaderInfoLog(vert, 512, NULL, log);
		std::cout << "Shader compilation failed for " << vertPath << "\n" << log << "\n";
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragShaderCode, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		glGetShaderInfoLog(frag, 512, NULL, log);
		std::cout << "Shader compilation failed for " << fragPath << "\n" << log << "\n";
	}

	Shader tempShader;

	tempShader.programID = glCreateProgram();
	glAttachShader(tempShader.programID, vert);
	glAttachShader(tempShader.programID, frag);
	glLinkProgram(tempShader.programID);

	glGetProgramiv(tempShader.programID, GL_LINK_STATUS, &succ);
	if (!succ)
	{
		glGetProgramInfoLog(tempShader.programID, 512, NULL, log);
		std::cout << "Shader linking failed!\n" << log << "\n";
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	this->_shaders.insert(std::pair<std::string, Shader>(name, tempShader));

	std::cout << "Successfully compiled shader " << name << " with a program ID of " << this->_shaders[name].programID << "!\n";
	return 0;
}

//passing back a pointer from scene which we obtained from here, we've gone full circle lol
void Shaders::utilise(const Shader * shader)
{
	glUseProgram(shader->programID);

	if (!shader->uniformFloats.empty())
	{
		std::map<const GLchar *, GLfloat>::const_iterator iter;
		iter = shader->uniformFloats.begin();
		while (iter != shader->uniformFloats.end())
		{
			glUniform1f(glGetUniformLocation(shader->programID, iter->first), iter->second);
			iter++;
		}
	}

	if (!shader->uniformVec3s.empty())
	{
		std::map<const GLchar *, glm::vec3>::const_iterator iter;
		iter = shader->uniformVec3s.begin();
		while (iter != shader->uniformVec3s.end())
		{
			glUniform3f(glGetUniformLocation(shader->programID, iter->first), iter->second.x, iter->second.y, iter->second.z);
			iter++;
		}
	}

	if (!shader->uniformMat4s.empty())
	{
		std::map<const GLchar *, glm::mat4>::const_iterator iter;
		iter = shader->uniformMat4s.begin();
		while (iter != shader->uniformMat4s.end())
		{
			glUniformMatrix4fv(glGetUniformLocation(shader->programID, iter->first), 1, GL_FALSE, glm::value_ptr(iter->second));
			iter++;
		}
	}
	
}

void Shaders::list()
{
	std::cout << "Here's a list of all loaded shaders:\n";
	std::unordered_map<std::string, Shader>::const_iterator iter;
	iter = this->_shaders.begin();
	while (iter != this->_shaders.end())
	{
		std::cout << "Shader with name " << iter->first << " and a program ID of " << iter->second.programID << "\n";
		iter++;
	}
	std::cout << "\n";
}
