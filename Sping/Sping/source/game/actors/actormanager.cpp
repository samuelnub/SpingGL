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
	this->player.spawn(this->_gamePtr, glm::vec3(0.0f, 0.0f, 0.0f));
}

void ActorManager::setupUniverse()
{
	//TODO: temp literal seed for the whole universe
	this->universe.spawn(this->_gamePtr, 1969);
}

void ActorManager::setupGalaxies()
{
	//TODO: stop making the string names literal you big fat bambi
	this->_gamePtr->shaders.compile("AGalaxy-Shader0", "res/shaders/testVert.glsl", "res/shaders/testFrag.glsl");

	this->_gamePtr->textures.store("AGalaxy-StarColor0", "res/textures/starColor.bmp", "diffuse");
}
