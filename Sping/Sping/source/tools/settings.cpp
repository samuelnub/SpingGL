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
	if (this->loadGraphicsSettings() != 0)
	{
		std::cout << "Couldn't load graphics settings!\n";
		exit(EXIT_FAILURE);
	}
	if (this->loadUserSettings() != 0)
	{
		std::cout << "Couldn't load user settings!\n";
		exit(EXIT_FAILURE);
	}

	return 0;
}

template<typename T>
int Settings::writeGraphicsSettings(const char *element, T value)
{
	const char *filePath = "res/settings/graphicssettings.xml";
	tinyxml2::XMLDocument doc;
	if (this->loadFile(doc, filePath, XMLDefaults::graphicsSettings) != 0)
	{
		return -1;
	}

	tinyxml2::XMLElement *rootNode = doc.FirstChildElement("root");
	this->checkElement(rootNode);

	rootNode = rootNode->FirstChildElement("GraphicsSettings");
	this->checkElement(rootNode);

	rootNode->FirstChildElement(element)->SetValue(value);

	if (doc.SaveFile(filePath) != 0)
	{
		std::cout << "Couldn't properly write settings to " << filePath << "!\n";
		return -1;
	}
	return 0;
}

template<typename T>
int Settings::writeUserSettings(std::string element, T value)
{
	const char *filePath = "res/settings/usersettings.xml";
	tinyxml2::XMLDocument doc;
	if (this->loadFile(doc, filePath, XMLDefaults::userSettings) != 0)
	{
		return -1;
	}

	tinyxml2::XMLElement *rootNode = doc.FirstChildElement("root");
	this->checkElement(rootNode);

	rootNode = rootNode->FirstChildElement("UserSettings");
	this->checkElement(rootNode);

	rootNode->FirstChildElement(element)->SetValue(value);

	if (doc.SaveFile(filePath) != 0)
	{
		std::cout << "Couldn't properly write settings to " << filePath << "!\n";
		return -1;
	}
	return 0;
}

int Settings::loadFile(tinyxml2::XMLDocument &doc, const char *filePath, const char *defaults)
{
	tinyxml2::XMLError xmlErr = doc.LoadFile(filePath);
	if (xmlErr != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not load the file requested at " << filePath << "! Attempting to create one...\a\n";
		doc.Parse(defaults);
		if (doc.SaveFile(filePath) != 0)
		{
			std::cout << "Couldn't create a default settings file at " << filePath << ", you're doomed\a\n";
			return -2;
		}
		return -1;
	}
	return 0;
}

void Settings::checkElement(tinyxml2::XMLElement * element)
{
	if (element == nullptr)
	{
		std::cout << "Uh oh! Couldn't obtain an element from a settings file! try deleting all the .xml files in ...res/settings/... and try running this program again to attempt to generate a fresh one!\a\n";
		exit(EXIT_FAILURE);
	}
}

int Settings::loadGraphicsSettings()
{
	const char *filePath = "res/settings/graphicssettings.xml";
	tinyxml2::XMLDocument doc;
	if (this->loadFile(doc, filePath, XMLDefaults::graphicsSettings) != 0)
	{
		return -1;
	}
	
	tinyxml2::XMLElement *rootNode = doc.FirstChildElement("root");
	this->checkElement(rootNode);

	rootNode = rootNode->FirstChildElement("GraphicsSettings");
	this->checkElement(rootNode);
	
	//lol
	rootNode->FirstChildElement("width")->QueryIntText(&this->graphicsSettings.width);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("height")->QueryIntText(&this->graphicsSettings.height);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("fullscreen")->QueryBoolText(&this->graphicsSettings.fullscreen);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("glMajor")->QueryIntText(&this->graphicsSettings.glMajor);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("glMinor")->QueryIntText(&this->graphicsSettings.glMinor);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("depthBits")->QueryIntText(&this->graphicsSettings.depthBits);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("stencilBits")->QueryIntText(&this->graphicsSettings.stencilBits);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("doublebuffer")->QueryBoolText(&this->graphicsSettings.doublebuffer);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("multisamples")->QueryIntText(&this->graphicsSettings.multisamples);
	this->checkElement(rootNode);

	this->graphicsSettings.name = rootNode->FirstChildElement("name")->FirstChild()->ToText()->Value();
	this->checkElement(rootNode);

	rootNode->FirstChildElement("maxActors")->QueryIntText(&this->graphicsSettings.maxActors);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("maxTexWidth")->QueryIntText(&this->graphicsSettings.maxTexWidth);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("maxTexHeight")->QueryIntText(&this->graphicsSettings.maxTexHeight);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("maxLod")->QueryIntText(&this->graphicsSettings.maxLod);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("universeMaxCount")->QueryIntText(&this->graphicsSettings.universeMaxCount);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("universeCellSize")->QueryFloatText(&this->graphicsSettings.universeCellSize);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("universeCellWidthCount")->QueryIntText(&this->graphicsSettings.universeCellWidthCount);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("galaxyMaxCount")->QueryIntText(&this->graphicsSettings.galaxyMaxCount);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("galaxyCellWidthCount")->QueryIntText(&this->graphicsSettings.galaxyCellWidthCount);
	this->checkElement(rootNode);

	std::cout << "Successfully loaded in " << filePath << "!\n";
	return 0;
}

int Settings::loadUserSettings()
{
	const char *filePath = "res/settings/usersettings.xml";
	tinyxml2::XMLDocument doc;
	if (this->loadFile(doc, filePath, XMLDefaults::userSettings) != 0)
	{
		return -1;
	}

	tinyxml2::XMLElement *rootNode = doc.FirstChildElement("root");
	this->checkElement(rootNode);

	rootNode = rootNode->FirstChildElement("UserSettings");
	this->checkElement(rootNode);

	//lol
	rootNode->FirstChildElement("zNear")->QueryFloatText(&this->userSettings.zNear);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("zFar")->QueryFloatText(&this->userSettings.zFar);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("fov")->QueryFloatText(&this->userSettings.fov);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("mouseLookSensitivity")->QueryFloatText(&this->userSettings.mouseLookSensitivity);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("rollSensitivity")->QueryFloatText(&this->userSettings.rollSensitivity);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("forward")->QueryIntText(&this->userSettings.forward);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("backward")->QueryIntText(&this->userSettings.backward);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("strafeLeft")->QueryIntText(&this->userSettings.strafeLeft);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("strafeRight")->QueryIntText(&this->userSettings.strafeRight);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("upward")->QueryIntText(&this->userSettings.upward);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("downward")->QueryIntText(&this->userSettings.downward);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("rollLeft")->QueryIntText(&this->userSettings.rollLeft);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("rollRight")->QueryIntText(&this->userSettings.rollRight);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("cursorLock")->QueryIntText(&this->userSettings.cursorLock);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("orientationReset")->QueryIntText(&this->userSettings.orientationReset);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("speedUp")->QueryIntText(&this->userSettings.speedUp);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("speedDown")->QueryIntText(&this->userSettings.speedDown);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("speedReset")->QueryIntText(&this->userSettings.speedReset);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("fovUp")->QueryIntText(&this->userSettings.fovUp);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("fovDown")->QueryIntText(&this->userSettings.fovDown);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("fovReset")->QueryIntText(&this->userSettings.fovReset);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("modifier")->QueryIntText(&this->userSettings.modifier);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("teleportHome")->QueryIntText(&this->userSettings.teleportHome);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("toggleInertia")->QueryIntText(&this->userSettings.toggleInertia);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("toggleWireframe")->QueryIntText(&this->userSettings.toggleWireframe);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("toggleDev")->QueryIntText(&this->userSettings.toggleDev);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("togglePause")->QueryIntText(&this->userSettings.togglePause);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("menuConfirm")->QueryIntText(&this->userSettings.menuConfirm);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("menuUp")->QueryIntText(&this->userSettings.menuUp);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("menuDown")->QueryIntText(&this->userSettings.menuDown);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("menuLeft")->QueryIntText(&this->userSettings.menuLeft);
	this->checkElement(rootNode);

	rootNode->FirstChildElement("menuRight")->QueryIntText(&this->userSettings.menuRight);
	this->checkElement(rootNode);

	std::cout << "Successfully loaded in " << filePath << "!\n";
	return 0;
}
