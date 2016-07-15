#include <game/states.h>
#include <iostream>

States::States()
{
}

States::~States()
{
}

void States::setup()
{
	std::fill(this->_states.begin(), this->_states.end(), false);

	this->set(State::DEV, true);
	//this->set(State::DEV_WIREFRAME, false);
	this->set(State::IN_GAME, true);
	//this->set(State::MENU_LOADING, false);
	//this->set(State::MENU_MAIN, false);
	//this->set(State::MENU_PAUSE, false);

	std::cout << "Just initialized the initial states of the game!\n";
}