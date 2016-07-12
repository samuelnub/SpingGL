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

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Renderable;

struct Mesh;
struct Shader;
struct Texture;

class Game;

//one instance of this should exist in game
class Scene
{
private:
	std::vector<Renderable *> _stagedRenderables;
	//ActorManager *_actors;
	//std::vector<Renderables> _stagedRenderables;

	Game *_gamePtr;

	std::map<Shader *, std::vector<Renderable *>> _stagedShaders;

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