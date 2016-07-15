#include <game/actors/player.h>
#include <game/game.h>

Player::Player()
{
}

Player::~Player()
{
}

void Player::spawn(Game *game, const glm::vec3 &spawnPos)
{
	this->_gamePtr = game;

	this->_fov = this->_gamePtr->settings.getUserSettings().fov;

	this->_camera.setup(
		this->_fov,
		(float)this->_gamePtr->settings.getGraphicsSettings().width / (float)this->_gamePtr->settings.getGraphicsSettings().height,
		this->_gamePtr->settings.getUserSettings().zNear,
		this->_gamePtr->settings.getUserSettings().zFar,
		spawnPos
		);

	this->_lastPos = spawnPos;
	this->_velocities = glm::vec3();
	this->_doInertia = false;
	this->_speed = 0.01;
}

void Player::update()
{
	if (this->_gamePtr->states.get(State::IN_GAME) == true)
	{
		this->doControls();
		this->doMouseLook();
		if (this->_doInertia == true)
		{
			this->applyInertia();
		}
	}
	if (this->_gamePtr->states.get(State::IN_GAME) == false)
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

void Player::doControls()
{
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().forward) == true)
	{
		this->_camera.translateLocal(glm::vec3(0.0f, 0.0f, -this->_speed * this->_gamePtr->input.getDelta()));
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().backward) == true)
	{
		this->_camera.translateLocal(glm::vec3(0.0f, 0.0f, this->_speed * this->_gamePtr->input.getDelta()));
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().strafeLeft) == true)
	{
		this->_camera.translateLocal(glm::vec3(-this->_speed * this->_gamePtr->input.getDelta(), 0.0f, 0.0f));
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().strafeRight) == true)
	{
		this->_camera.translateLocal(glm::vec3(this->_speed * this->_gamePtr->input.getDelta(), 0.0f, 0.0f));
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().upward) == true)
	{
		this->_camera.translateLocal(glm::vec3(0.0f, this->_speed * this->_gamePtr->input.getDelta(), 0.0f));
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().downward) == true)
	{
		this->_camera.translateLocal(glm::vec3(0.0f, -this->_speed * this->_gamePtr->input.getDelta(), 0.0f));
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().rollLeft) == true)
	{
		this->_camera.roll(-0.008f * this->_gamePtr->input.getDelta());
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().rollRight) == true)
	{
		this->_camera.roll(0.008f * this->_gamePtr->input.getDelta());
	}

	//fov and speed stuff
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().fovUp) == true)
	{
		this->_fov *= 1.02;

		this->_camera.setFOV(this->_fov);
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().fovDown) == true)
	{
		this->_fov /= 1.02;

		this->_camera.setFOV(this->_fov);
	}
	if (this->_gamePtr->input.wasKeyPressed(this->_gamePtr->settings.getUserSettings().fovReset) == true)
	{
		this->_fov = this->_gamePtr->settings.getUserSettings().fov;

		this->_camera.setFOV(this->_fov);
	}

	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().speedUp) == true)
	{
		this->_speed *= 1.02;
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().speedDown) == true)
	{
		if (this->_speed / 1.02 < 0.000001f)
		{
			this->_speed = 0.000001f;
		}
		this->_speed /= 1.02;
	}
	if (this->_gamePtr->input.wasKeyPressed(this->_gamePtr->settings.getUserSettings().speedReset) == true)
	{
		this->_speed = 0.01;
	}
}

void Player::doMouseLook()
{
	//really useful function, thanks sdl
	SDL_SetRelativeMouseMode(SDL_TRUE);
	this->_camera.yaw(
		this->_gamePtr->settings.getUserSettings().mouseLookSensitivity *
		-this->_gamePtr->input.getCursorMovedX());
	this->_camera.pitch(
		this->_gamePtr->settings.getUserSettings().mouseLookSensitivity *
		-this->_gamePtr->input.getCursorMovedY());
}

void Player::applyInertia()
{
}
