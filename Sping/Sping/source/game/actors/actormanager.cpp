#include <game/actors/actormanager.h>
#include <game/game.h>
#include <iostream>

ActorManager::ActorManager()
{
}

ActorManager::~ActorManager()
{
}

int ActorManager::setup(Game *game)
{
	this->_gamePtr = game;

	//call setup funcs of generators
	this->galaxygen.setup(this->_gamePtr);

	//call specific setup funcs of actors which are declared in the protected section
	this->setupPlayer();
	this->setupUniverse();
	this->setupGalaxies();

	std::cout << "Successfully set up the Actor Manager!\n";
	return 0;
}

void ActorManager::update()
{
	this->player.update();

}

void ActorManager::setupPlayer()
{
	this->player = APlayer(
		this->_gamePtr,
		Transform(
			this->_gamePtr,
			DrawPriority::UNIVERSE,
			Vec3Acc(0.0, 0.0, 0.0),
			true
			)
		);
}

void ActorManager::setupUniverse()
{
	//TODO: actor manager n stuff
}

void ActorManager::setupGalaxies()
{
	
}
