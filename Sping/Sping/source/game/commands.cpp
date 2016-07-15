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

//generally legacy class and functions, only for core commands to be impulsed, the rest? you'd probably have to dig through and find whos reading input and doing stuff.
void Commands::impuseEveryFrame()
{
	
}

void Commands::impulseKeyDown()
{
	//"system" pause menu sorta thing and "quit"
	if (this->_gamePtr->input.wasKeyPressed(this->_gamePtr->settings.getUserSettings().togglePause))
	{
		this->_gamePtr->states.toggle(State::MENU_PAUSE);
		this->_gamePtr->states.toggle(State::IN_GAME);
	}

	if (this->_gamePtr->input.wasKeyPressed(this->_gamePtr->settings.getUserSettings().menuConfirm))
	{
		if (this->_gamePtr->states.get(State::IN_GAME) == 0 && this->_gamePtr->states.get(State::MENU_PAUSE) == 1)
		{
			SDL_Quit();
			exit(EXIT_SUCCESS);
		}
	}
	//dev mode toggle
	if (this->_gamePtr->input.wasKeyPressed(this->_gamePtr->settings.getUserSettings().toggleDev) && this->_gamePtr->input.isKeyHeld(this->_gamePtr->settings.getUserSettings().modifier))
	{
		this->_gamePtr->states.set(State::DEV, !this->_gamePtr->states.get(State::DEV));
		std::cout << "Toggled dev mode to " << this->_gamePtr->states.get(State::DEV) << "!\n";
	}


}

void Commands::impulseKeyHeld()
{
	
}

void Commands::impulseKeyUp()
{

}

void Commands::impulseMouseMotion()
{

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
