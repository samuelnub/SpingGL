#include <game/states.h>
#include <iostream>

States::States()
{
}

States::~States()
{
	this->_states.clear();
}

//naming convention for keys: lower case separated by underscores
void States::setup()
{
	this->_states["loading"] = true;
	this->_states["main_menu"] = false;
	this->_states["in_game"] = false;
	this->_states["paused"] = true;
	this->_states["pause_menu"] = false;

	std::cout << "Just initialized the initial states of the game!\n";
	this->list();
}

bool States::get(const std::string & key)
{
	return this->_states.at(key);
}

void States::set(const std::string & key, const bool state)
{
	this->_states.at(key) = state;
}

void States::toggle(const std::string & key)
{
	this->_states.at(key) = !this->_states.at(key);
}

void States::add(const std::string & key, const bool state)
{
	this->_states[key] = state;
}

void States::list()
{
	std::cout << "Here are all the current game states:\n";

	std::map<std::string, bool>::const_iterator iter;
	iter = this->_states.begin();
	while (iter != this->_states.end())
	{
		std::cout << iter->first << " is " << iter->second << "\n";
		iter++;
	}
}
