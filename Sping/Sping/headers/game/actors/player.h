#pragma once
#ifndef GAME_ACTORS_PLAYER_H
#define GAME_ACTORS_PLAYER_H

#include <tools/camera.h>

class Game;

//a type of actor, apparently, who would'a thunk. the "in-game" personna of a player. there should be one instance of this in the actormanager, this guy's special.
class Player
{
private:
	Game *_gamePtr;

	float _fov;

	bool _doInertia;

	glm::vec3 _lastPos;
	glm::vec3 _velocities;

	float _speed;

public:
	Player();
	~Player();

	void spawn(Game *game, const glm::vec3 &spawnPos);

	void update();

	Camera _camera; //TODO: stop making these public you idiot

protected:
	void doControls();
	void doMouseLook();

	void applyInertia();

};

#endif