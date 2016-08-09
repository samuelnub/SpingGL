#include <tools/settings.h>
#include <iostream>
#include <globals/xmldefaults.h>

Settings::Settings()
{
}

Settings::~Settings()
{
}

int Settings::setup()
{
	this->_filePath = "res/settings/settings.xml";

	if (this->load() != 0)
	{
		std::cout << "Couldn't load settings!\n";
		return -1;
	}
	return 0;
}

int Settings::load()
{
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLError xmlError = xmlDoc.LoadFile(this->_filePath);
	if (xmlError != 0)
	{
		std::cout << "Couldn't load XML file from " << this->_filePath << "!\n";
		return -1;
	}


	tinyxml2::XMLElement *xmlEle = xmlDoc.FirstChildElement("settings");
	if (xmlEle != nullptr)
	{
		xmlEle = xmlEle->FirstChildElement("setting");
		if (xmlEle != nullptr)
		{
			while (xmlEle)
			{
				std::string settingName = xmlEle->Attribute("name");

				std::string settingType = xmlEle->Attribute("type");
				
				//doesn't support switch cases for strings :(
				if (settingType == "BOOL")
				{
					xmlEle->QueryBoolAttribute("value", &this->_settings[settingName]._bool);
				}
				else if (settingType == "INT")
				{
					xmlEle->QueryIntAttribute("value", &this->_settings[settingName]._int);
				}
				else if (settingType == "FLOAT")
				{
					xmlEle->QueryFloatAttribute("value", &this->_settings[settingName]._float);
				}
				else if (settingType == "DOUBLE")
				{
					xmlEle->QueryDoubleAttribute("value", &this->_settings[settingName]._double);
				}
				else if (settingType == "STRING")
				{
					this->_settings[settingName]._string = xmlEle->Attribute("value");
				}

				if(xmlEle == nullptr)
				{ 
					return -1;
				}
				std::cout << "Got the Setting " << settingName << "!\n";
			}
		}
	}

	return 0;
}
