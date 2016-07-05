#pragma once
#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <SDL2/SDL.h>

#include <game/states.h>

class Game
{
private:
	SDL_Window *_window;

public:
	//constructor sets up all initial stuff, then calls the loop func
	Game(SDL_Window *window);
	~Game();
	void loop();

	//lord forgive me for my public member variables
	States states;

protected:
	

};

#endif