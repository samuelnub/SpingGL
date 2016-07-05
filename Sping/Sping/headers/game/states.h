#pragma once
#ifndef GAME_STATES_H
#define GAME_STATES_H

#include <string>
#include <map>

//contains all the useless bools which control everything, game class should have one of this as its members
class States
{
private:
	std::map<std::string, bool> _states;

public:
	States();
	~States();

	void setup();
	bool get(const std::string &key);
	void set(const std::string &key, const bool state); //does not add if it doesnt exist
	void toggle(const std::string &key);
	void add(const std::string &key, const bool state); //if what you wanna add already exists, itll just set the state you give it
	void list(); //in case you need to know

protected:


};

#endif