#include <game/actors/player.h>
#include <game/game.h>

APlayer::APlayer()
{
}

APlayer::~APlayer()
{
}

void APlayer::spawn(Game *game, const glm::vec3 &spawnPos)
{
	this->_gamePtr = game;
	this->_gamePtr->uuidgen.gen(this->_uuid);
	this->_spawnPos = spawnPos;
	
	this->_fov = this->_gamePtr->settings.getUserSettings().fov;
	//TODO: fix aspect when fullscreen
	this->_camera.setup(
		this->_fov,
		(float)this->_gamePtr->settings.getGraphicsSettings().width / (float)this->_gamePtr->settings.getGraphicsSettings().height,
		this->_gamePtr->settings.getUserSettings().zNear,
		this->_gamePtr->settings.getUserSettings().zFar,
		this->_spawnPos
		);

	this->_lastPos = spawnPos;
	this->_velocities = glm::vec3();
	this->_doInertia = false;
	this->_speed = 0.01;
}

void APlayer::update()
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

void APlayer::doControls()
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
		this->_camera.roll(-0.005f * this->_gamePtr->input.getDelta());
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().rollRight) == true)
	{
		this->_camera.roll(0.005f * this->_gamePtr->input.getDelta());
	}

	//fov and speed stuff
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().fovUp) == true)
	{
		this->_fov *= (double)1.02;
		std::cout << "FOV of player is now " << this->_fov << "!\n";
		this->_camera.setFOV(this->_fov);
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().fovDown) == true)
	{
		this->_fov *= (double)0.98;
		std::cout << "FOV of player is now " << this->_fov << "!\n";
		this->_camera.setFOV(this->_fov);
	}
	if (this->_gamePtr->input.wasKeyPressed(this->_gamePtr->settings.getUserSettings().fovReset) == true)
	{
		this->_fov = this->_gamePtr->settings.getUserSettings().fov;

		this->_camera.setFOV(this->_fov);
	}

	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().speedUp) == true)
	{
		this->_speed *= (double)1.05;
		std::cout << "Speed multiplier is now " << this->_speed << "!\n";
	}
	if (this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().speedDown) == true)
	{
		if (this->_speed / 1.02 < 0.000001f)
		{
			this->_speed = 0.000001f;
		}
		this->_speed *= (double)0.95;
		std::cout << "Speed multiplier is now " << this->_speed << "!\n";
	}
	if (this->_gamePtr->input.wasKeyPressed(this->_gamePtr->settings.getUserSettings().speedReset) == true)
	{
		this->_speed = 0.01;
	}


	//TODO: just test
	if (this->_gamePtr->input.wasKeyPressed(SDL_SCANCODE_P) == true)
	{
		std::cout << "Player at: " << this->_camera.getPos().x << ", " << this->_camera.getPos().y << ", " << this->_camera.getPos().z << "\n";
	}
	if (this->_gamePtr->input.wasKeyPressed(this->_gamePtr->settings.getUserSettings().teleportHome) == true)
	{
		this->_camera.setPos(glm::vec3(this->_spawnPos));
		std::cout << "Teleported home!\n";
	}
}

void APlayer::doMouseLook()
{
	//really useful function, thanks sdl
	SDL_SetRelativeMouseMode(SDL_TRUE);
	this->_camera.yaw(
		(this->_gamePtr->settings.getUserSettings().mouseLookSensitivity * this->_fov) *
		(-this->_gamePtr->input.getCursorMovedX() * 0.01f));
	this->_camera.pitch(
		(this->_gamePtr->settings.getUserSettings().mouseLookSensitivity * this->_fov) *
		(-this->_gamePtr->input.getCursorMovedY() * 0.01f));
}

void APlayer::applyInertia()
{
}
