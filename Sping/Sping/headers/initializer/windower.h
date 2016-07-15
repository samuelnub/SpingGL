#pragma once
#ifndef INITIALIZER_WINDOWER_H
#define INITIALIZER_WINDOWER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <string>

class Game;

class Window
{
private:
	Game *_gamePtr;

	SDL_GLContext _context;

	SDL_DisplayMode _displayMode;

	SDL_Renderer *_gottenRenderer; //the dimensions of the window actually created

public:
	Window();
	~Window(); //deletes sdl window context and cleans up some shizmo

	void setup(Game *game);

	int createdWidth;
	int createdHeight;

	SDL_Window *_window;

protected:

};

#endif