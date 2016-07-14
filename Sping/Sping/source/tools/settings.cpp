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
	this->checkElement(rootNode, filePath);

	rootNode = rootNode->FirstChildElement("GraphicsSettings");
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement(element)->SetValue(value);

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
	this->checkElement(rootNode, filePath);

	rootNode = rootNode->FirstChildElement("UserSettings");
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement(element)->SetValue(value);

	return 0;
}

int Settings::loadFile(tinyxml2::XMLDocument &doc, const char *filePath, const char *defaults)
{
	tinyxml2::XMLError xmlErr = doc.LoadFile(filePath);
	if (xmlErr != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Could not load the file requested at " << filePath << "! Attempting to create one...\a\n";
		doc.Parse(defaults);
		doc.SaveFile(filePath);
		return -1;
	}
	return 0;
}

void Settings::checkElement(tinyxml2::XMLElement * element, const char * filePath)
{
	if (element == nullptr)
	{
		std::cout << "Uh oh! Couldn't obtain an element from " << filePath << ", try deleting that file and running this program again to attempt to generate a fresh one!\a\n";
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
	this->checkElement(rootNode, filePath);

	rootNode = rootNode->FirstChildElement("GraphicsSettings");
	this->checkElement(rootNode, filePath);
	
	//lol
	rootNode->FirstChildElement("width")->QueryIntText(&this->graphicsSettings.width);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("height")->QueryIntText(&this->graphicsSettings.height);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("fullscreen")->QueryBoolText(&this->graphicsSettings.fullscreen);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("glMajor")->QueryIntText(&this->graphicsSettings.glMajor);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("glMinor")->QueryIntText(&this->graphicsSettings.glMinor);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("depthBits")->QueryIntText(&this->graphicsSettings.depthBits);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("stencilBits")->QueryIntText(&this->graphicsSettings.stencilBits);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("doublebuffer")->QueryBoolText(&this->graphicsSettings.doublebuffer);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("multisamples")->QueryIntText(&this->graphicsSettings.multisamples);
	this->checkElement(rootNode, filePath);

	this->graphicsSettings.name = rootNode->FirstChildElement("name")->FirstChild()->ToText()->Value();
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("maxActors")->QueryIntText(&this->graphicsSettings.maxActors);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("maxTexWidth")->QueryIntText(&this->graphicsSettings.maxTexWidth);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("maxTexHeight")->QueryIntText(&this->graphicsSettings.maxTexHeight);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("maxLod")->QueryIntText(&this->graphicsSettings.maxLod);
	this->checkElement(rootNode, filePath);


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
	this->checkElement(rootNode, filePath);

	rootNode = rootNode->FirstChildElement("UserSettings");
	this->checkElement(rootNode, filePath);

	//lol
	rootNode->FirstChildElement("zNear")->QueryFloatText(&this->userSettings.zNear);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("zFar")->QueryFloatText(&this->userSettings.zFar);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("fov")->QueryFloatText(&this->userSettings.fov);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("mouseLookSensitivity")->QueryFloatText(&this->userSettings.mouseLookSensitivity);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("rollSensitivity")->QueryFloatText(&this->userSettings.rollSensitivity);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("forward")->QueryIntText(&this->userSettings.forward);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("backward")->QueryIntText(&this->userSettings.backward);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("strafeLeft")->QueryIntText(&this->userSettings.strafeLeft);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("strafeRight")->QueryIntText(&this->userSettings.strafeRight);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("upward")->QueryIntText(&this->userSettings.upward);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("downward")->QueryIntText(&this->userSettings.downward);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("rollLeft")->QueryIntText(&this->userSettings.rollLeft);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("rollRight")->QueryIntText(&this->userSettings.rollRight);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("orientationReset")->QueryIntText(&this->userSettings.orientationReset);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("speedUp")->QueryIntText(&this->userSettings.speedUp);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("speedDown")->QueryIntText(&this->userSettings.speedDown);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("speedReset")->QueryIntText(&this->userSettings.speedReset);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("fovUp")->QueryIntText(&this->userSettings.fovUp);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("fovDown")->QueryIntText(&this->userSettings.fovDown);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("fovReset")->QueryIntText(&this->userSettings.fovReset);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("teleportHome")->QueryIntText(&this->userSettings.teleportHome);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("toggleInertia")->QueryIntText(&this->userSettings.toggleInertia);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("toggleWireframe")->QueryIntText(&this->userSettings.toggleWireframe);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("toggleDev")->QueryIntText(&this->userSettings.toggleDev);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("togglePause")->QueryIntText(&this->userSettings.togglePause);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("menuConfirm")->QueryIntText(&this->userSettings.menuConfirm);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("menuUp")->QueryIntText(&this->userSettings.menuUp);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("menuDown")->QueryIntText(&this->userSettings.menuDown);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("menuLeft")->QueryIntText(&this->userSettings.menuLeft);
	this->checkElement(rootNode, filePath);

	rootNode->FirstChildElement("menuRight")->QueryIntText(&this->userSettings.menuRight);
	this->checkElement(rootNode, filePath);

	std::cout << "Successfully loaded in " << filePath << "!\n";
	return 0;
}
