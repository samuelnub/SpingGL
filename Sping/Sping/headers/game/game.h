#pragma once
#ifndef GAME_GAME_H
#define GAME_GAME_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <game/states.h>
#include <game/input.h>
#include <initializer/windower.h>

#include <tools/meshes.h>
#include <tools/shaders.h>
#include <tools/textures.h>

#include <render/scene.h>

class Game
{
private:

public:
	//constructor sets up all initial stuff, then calls the loop func
	Game();
	~Game();
	void loop();

	//lord forgive me for my public member variables
	Window window;
	States states;
	Input input;

	Meshes meshes;
	Shaders shaders;
	Textures textures;

	Scene scene;

protected:
	

};

#endif