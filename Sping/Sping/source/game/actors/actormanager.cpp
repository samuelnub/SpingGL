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

	this->player.spawn(this->_gamePtr, glm::vec3(0.0f, 0.0f, -1.0f));

	std::cout << "Successfully set up the Actor Manager!\n";
	return 0;
}

void ActorManager::update()
{
	this->player.update();

}
