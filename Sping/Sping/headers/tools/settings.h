#pragma once
#ifndef TOOLS_SETTINGS_H
#define TOOLS_SETTINGS_H

#include <tinyxml2/tinyxml2.h>

#include <unordered_map>
#include <string>

//too much of a wuss to use unions :(
struct DataHolder
{
	bool _bool;
	int _int;
	float _float;
	double _double;
	std::string _string;
};

class Settings
{
private:
	std::unordered_map<std::string, DataHolder> _settings;

	const char *_filePath;

public:
	Settings();
	~Settings();

	int setup();

	inline const DataHolder *get(std::string setting)
	{
		try
		{
			return &this->_settings.at(setting);
		}
		catch (std::exception err)
		{
			return nullptr;
		}
	}

protected:
	//setup() should load these internally
	int load();
	
};

#endif


