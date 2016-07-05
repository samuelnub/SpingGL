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

int States::get(const std::string & key)
{
	try
	{
		return this->_states.at(key);
	}
	catch (std::exception &err)
	{
		std::cout << key << " doesn't exist in this map! Can't get anything!\n";
		return -1; //doesnt exist
	}
}

void States::set(const std::string & key, const bool state)
{
	std::cout << key << " was set to " << state << "!\n";
	this->_states.at(key) = state;
}

void States::toggle(const std::string & key)
{
	std::cout << key << " was toggled to " << !this->_states.at(key) << "!\n";
	this->_states.at(key) = !this->_states.at(key);
}

void States::add(const std::string & key, const bool state)
{
	std::cout << key << " was added to the states map and was set to " << state << "!\n";
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
