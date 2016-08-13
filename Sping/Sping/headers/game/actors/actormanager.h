#pragma once
#ifndef GAME_ACTORS_ACTOR_MANAGER_H
#define GAME_ACTORS_ACTOR_MANAGER_H

#include <vector>
#include <octree/octree.h>

#include <boost/uuid/uuid.hpp>

//includes for actor types
#include <game/actors/player.h>

//includes for generators that these actors utilize
#include <tools/generators/galaxygen.h>

class Game;

//factory and warehouse of all actors (an actor composes of a renderable)
class ActorManager
{
private:
	Game *_gamePtr = nullptr;

public:
	ActorManager();
	~ActorManager();

	//setting up should initialise arrays, spawn specialties like the player etc.
	int setup(Game *game);

	//runs through arrays and calls each one's update function (they should be inlined)
	void update();

	//===============================================
	//============Containers for AActors=============
	//===============================================
	APlayer player;
	
	//the main thing that actor classes will store is a uniqueptr to an octree in most cases, other than that, they just have pointers to the data they need

	//===============================================
	//============Generators for AActors=============
	//===============================================
	GalaxyGenerator galaxygen;

protected:
	//protected funcs called by main setup() to do the initial setting up of each appropriate actor (certain actors may need certain shaders set up... etc)
	void setupPlayer();
	void setupUniverse();
	void setupGalaxies();

};

#endif