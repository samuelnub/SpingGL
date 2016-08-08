#pragma once
#ifndef GAME_COMMANDS_H
#define GAME_COMMANDS_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>

class Game;

//TODO: these are defaults when you first start up the program, should be overridden eventually by reading from controlsettings.xml
//this namespace only specifies controls, as the name implies. it does not specify any form of setting such as fov or mouse sensitivity, but simply - keybindings

//a wordy class, contains all "controls" to send out to the appropriate 
class Commands
{
private:
	Game *_gamePtr = nullptr;

public:
	Commands();
	~Commands();

	int setup(Game *game);

	//reflecting input's processing functions
	void impuseEveryFrame();

	void impulseKeyDown();

	void impulseKeyHeld();

	void impulseKeyUp();

	void impulseMouseMotion();

	void impulseMouseScroll();

	void impulseMouseButtonDown();

	void impulseMouseButtonUp();

protected:
	//TODO: do it
	void loadXML(const char *filePath);

};

#endif