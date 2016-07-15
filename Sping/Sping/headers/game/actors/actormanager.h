#pragma once
#ifndef GAME_ACTORS_ACTOR_MANAGER_H
#define GAME_ACTORS_ACTOR_MANAGER_H

#include <vector>

//includes for actor types
#include <game/actors/player.h>

class Game;

//factory and warehouse of all actors (an actor composes of a renderable)
class ActorManager
{
private:
	Game *_gamePtr;

	//TODO: could generate ID's based on position to make it unique
	uint64_t _IDcount;
	
public:
	ActorManager();
	~ActorManager();

	//setting up should initialise arrays, spawn specialties like the player etc.
	int setup(Game *game);

	//runs through arrays and calls each one's update function (they should be inlined)
	void update();

	//=========================================================================
	//=============Arrays for actor types, or singular special ones============
	//=========================================================================
	Player player;
	//sorry theyre public

protected:


};

#endif