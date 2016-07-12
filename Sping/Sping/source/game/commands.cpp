#include <game/commands.h>
#include <game/game.h>
#include <iostream>

Commands::Commands()
{
}

Commands::~Commands()
{
}

int Commands::setup(Game *game)
{
	this->_gamePtr = game;

	this->loadXML("res/settings/controlsettings.xml");

	std::cout << "Just set up the command system!\n";

	return 0;
}

//==============================================
//====================oh boy====================
//==============================================
namespace Controls
{
	namespace System
	{
		SDL_Scancode ToggleQuitPrompt = SDL_SCANCODE_ESCAPE;
		SDL_Scancode ConfirmQuit = SDL_SCANCODE_RETURN;
		SDL_Scancode ControlMod = SDL_SCANCODE_LCTRL;
	}

	namespace Eyes
	{
		SDL_Scancode Forward = SDL_SCANCODE_W;
		SDL_Scancode Backward = SDL_SCANCODE_S;
		SDL_Scancode StrafeLeft = SDL_SCANCODE_A;
		SDL_Scancode StrafeRight = SDL_SCANCODE_D;

		SDL_Scancode Upward = SDL_SCANCODE_R;
		SDL_Scancode Downward = SDL_SCANCODE_F;
		SDL_Scancode RollLeft = SDL_SCANCODE_Q;
		SDL_Scancode RollRight = SDL_SCANCODE_E;

		SDL_Scancode SpeedUp = SDL_SCANCODE_T;
		SDL_Scancode SpeedDown = SDL_SCANCODE_G;
		SDL_Scancode SpeedReset = SDL_SCANCODE_V;

		SDL_Scancode FovHigher = SDL_SCANCODE_PAGEUP;
		SDL_Scancode FowLower = SDL_SCANCODE_PAGEDOWN;
		SDL_Scancode FovReset = SDL_SCANCODE_HOME;

		SDL_Scancode WireframeToggle = SDL_SCANCODE_W; //note how it overlaps with Forward, but it doesn't matter, because this will require the ControlMod to && be true

		int LookMouseButton = SDL_BUTTON_LEFT;

	}


}

//tbh, keeping them all here is easier to manage than searching through every gosh darn source file to see "which class is stealing my input?!"
void Commands::impulseKeyDown()
{
	//"system" pause menu sorta thing and "quit"
	if (this->_gamePtr->input.wasKeyPressed(Controls::System::ToggleQuitPrompt))
	{
		this->_gamePtr->states.toggle("in_game");
		this->_gamePtr->states.toggle("paused");
	}

	if (this->_gamePtr->input.wasKeyPressed(Controls::System::ConfirmQuit))
	{
		if (this->_gamePtr->states.get("in_game") == 0 && this->_gamePtr->states.get("paused") == 1)
		{
			SDL_Quit();
			exit(EXIT_SUCCESS);
		}
	}
	//wireframe toggle
	if (this->_gamePtr->input.wasKeyPressed(Controls::Eyes::WireframeToggle) && this->_gamePtr->input.isKeyHeld(Controls::System::ControlMod))
	{
		this->_gamePtr->states.toggle("wireframe");
		//too lazy to send out some external command to scene.render, so ill just toggle it here
		if (this->_gamePtr->states.get("wireframe") == 1)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (this->_gamePtr->states.get("wireframe") == 0)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	
}

void Commands::impulseKeyHeld()
{
	//ingame controls
	if (this->_gamePtr->states.get("in_game") == 1)
	{
		if (this->_gamePtr->input.isKeyHeld(Controls::Eyes::Forward))
		{
			this->_gamePtr->eyes.move(FORWARD);
		}

		if (this->_gamePtr->input.isKeyHeld(Controls::Eyes::Backward))
		{
			this->_gamePtr->eyes.move(BACKWARD);
		}

		if (this->_gamePtr->input.isKeyHeld(Controls::Eyes::StrafeLeft))
		{
			this->_gamePtr->eyes.move(STRAFE_LEFT);
		}

		if (this->_gamePtr->input.isKeyHeld(Controls::Eyes::StrafeRight))
		{
			this->_gamePtr->eyes.move(STRAFE_RIGHT);
		}

		if (this->_gamePtr->input.isKeyHeld(Controls::Eyes::Upward))
		{
			this->_gamePtr->eyes.move(UP);
		}

		if (this->_gamePtr->input.isKeyHeld(Controls::Eyes::Downward))
		{
			this->_gamePtr->eyes.move(DOWN);
		}
		//mouse controls below somewhere lol
	}

}

void Commands::impulseKeyUp()
{

}

void Commands::impulseMouseMotion()
{
	//ingame controls
	if (this->_gamePtr->states.get("in_game") == 1)
	{
		//if (this->_gamePtr->input.wasMouseButtonPressed(Controls::Eyes::LookMouseButton))
		{
			std::cout << "Moving eyes!\n";
			this->_gamePtr->eyes.setPos(
				this->_gamePtr->input.getCursorX(),
				this->_gamePtr->input.getCursorY());
			this->_gamePtr->eyes.move2D(
				this->_gamePtr->input.getCursorX(),
				this->_gamePtr->input.getCursorY());
		}
	}

}

void Commands::impulseMouseScroll()
{

}

void Commands::impulseMouseButtonDown()
{

}

void Commands::impulseMouseButtonUp()
{

}



void Commands::loadXML(const char * filePath)
{
}
