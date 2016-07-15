#pragma once
#ifndef GAME_STATES_H
#define GAME_STATES_H

#include <array>
#include <iostream>

enum class State //a lot are redundant for now, just showing
{
	IN_GAME,
	MENU_MAIN,
	MENU_PAUSE,
	MENU_LOADING,
	DEV,
	DEV_WIREFRAME,


	ENUM_SIZE
};


//contains all the useless bools which controls main stuff, game class should have one of this as its members
class States
{
private:
	//c style casting always makes me tingle on the inside hehe
	std::array<bool, (size_t)State::ENUM_SIZE> _states;

public:
	States();
	~States();

	void setup();
	
	inline bool get(State state)
	{
		return this->_states.at((size_t)state);
	}

	inline void set(State state, bool value)
	{
		this->_states.at((size_t)state) = value;
		std::cout << "State enum " << (int)state << " has been set to " << value << "!\n";
	}

	inline void toggle(State state)
	{
		this->_states.at((size_t)state) = !this->_states.at((size_t)state);
		std::cout << "State enum " << (int)state << " has been toggled to " << this->_states.at((size_t)state) << "!\n";
	}

protected:


};

#endif