#pragma once
#ifndef INITIALIZER_WINDOWER_H
#define INITIALIZER_WINDOWER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <string>

class Window
{
private:
	SDL_GLContext _context;

	int _glMajor;
	int _glMinor;

	int _depthBits;
	int _stencilBits;
	bool _doublebuffer;
	int _multisamples;

	std::string _name;

	int _width;
	int _height;
	bool _fullscreen;

	int _failedAttempts;

	SDL_DisplayMode _displayMode;
	SDL_Renderer *_gottenRenderer;

	int _createdWidth;
	int _createdHeight;

public:
	Window();
	~Window(); //deletes sdl window context and cleans up some shizmo

	void setup();

	SDL_Window *_window;
protected:
	void loadXML(const char *filePath);

};

#endif