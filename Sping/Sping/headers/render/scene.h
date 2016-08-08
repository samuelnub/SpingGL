#pragma once
#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <vector>
#include <map>
#include <array>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <render/renderable.h>

struct Mesh;
struct Shader;
struct Texture;

class Game;

//one instance of this should exist in game
class Scene
{
private:
	Game *_gamePtr = nullptr;
	// TODO: use hashmap with bools instead
	std::vector<Renderable *> _stagedRenderables;
	
	std::array<std::map<Shader *, std::vector<Renderable *>>, 
	static_cast<int>(DrawPriority::ENUM_COUNT)> _staged;

public:
	Scene();
	~Scene();

	int setup(Game *game);

	int stage(Renderable *renderable);

	int unstage(Renderable *renderable);

	bool isStaged(Renderable *renderable);

	void draw();

protected:
	void flush();

};

#endif