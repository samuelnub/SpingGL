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

	ENUM_COUNT
};


//contains all the useless bools which controls main stuff, game class should have one of this as its members
class States
{
private:
	std::array<bool, static_cast<int>(State::ENUM_COUNT)> _states;

public:
	States();
	~States();

	void setup();
	
	inline bool get(State state)
	{
		return this->_states[static_cast<int>(state)];
	}

	inline void set(State state, bool value)
	{
		this->_states[static_cast<int>(state)] = value;
		std::cout << "State enum " << static_cast<int>(state) << " has been set to " << value << "!\n";
	}

	inline void toggle(State state)
	{
		this->_states[static_cast<int>(state)] = !this->_states[static_cast<int>(state)];
		std::cout << "State enum " << static_cast<int>(state) << " has been toggled to " << this->_states[static_cast<int>(state)] << "!\n";
	}

protected:


};

#endif