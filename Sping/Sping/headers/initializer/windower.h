#pragma once
#ifndef INITIALIZER_WINDOWER_H
#define INITIALIZER_WINDOWER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Window
{
private:
	SDL_Window *_window;
	SDL_GLContext _context;

public:
	Window();
	~Window(); //deletes sdl window context and cleans up some shizmo

protected:


};

#endif