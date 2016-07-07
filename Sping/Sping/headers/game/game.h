#pragma once
#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <SDL2/SDL.h>
#include <game/states.h>
#include <game/input.h>
#include <initializer/windower.h>

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
protected:
	

};

#endif