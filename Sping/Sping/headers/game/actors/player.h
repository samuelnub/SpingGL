#pragma once
#ifndef GAME_ACTORS_PLAYER_H
#define GAME_ACTORS_PLAYER_H

#include <tools/camera.h>
#include <boost/uuid/uuid.hpp>

class Game;

//a type of actor, apparently, who would'a thunk. the "in-game" personna of a player. there should be one instance of this in the actormanager, this guy's special.
class APlayer
{
private:
	Game *_gamePtr = nullptr;

	boost::uuids::uuid _uuid;

	float _fov;

	bool _doInertia;

	glm::vec3 _spawnPos;
	glm::vec3 _lastPos;
	glm::vec3 _velocities;

	float _speed;

public:
	APlayer();
	~APlayer();

	void spawn(Game *game, const glm::vec3 &spawnPos);

	void update();

	inline boost::uuids::uuid getUUID()
	{
		return this->_uuid;
	}

	Camera _camera; //TODO: stop making these public you idiot

protected:
	void doControls();
	void doMouseLook();

	void applyInertia();

};

#endif