#pragma once
#ifndef TOOLS_SETTINGS_H
#define TOOLS_SETTINGS_H

#include <tinyxml2/tinyxml2.h>

#include <string>

//========================================================
//================graphics settings values================
//========================================================
struct GraphicsSettings
{
	int width; //width specified when not fullscreen
	int height; //height specified when not fullscreen
	bool fullscreen; //width and height wont be used if its true, will use native screen resolution

	int glMajor;
	int glMinor;
	int depthBits;
	int stencilBits;
	bool doublebuffer;
	int multisamples;
	std::string name; //window name lol

	int maxActors;
	int maxTexWidth; //TODO: for eventual texture generation
	int maxTexHeight;
	int maxLod; //max recursion level for creating lod meshes
};

//========================================================
//==================user settings values==================
//========================================================
struct UserSettings
{
	float zNear;
	float zFar;
	float fov;

	float mouseLookSensitivity;
	float rollSensitivity;

	//keybind preferences, sdl_scancodes are just defined as ints, so ¯\_(?)_/¯
	int forward;
	int backward;
	int strafeLeft;
	int strafeRight;
	int upward;
	int downward;
	int rollLeft;
	int rollRight;

	int orientationReset;

	int speedUp;
	int speedDown;
	int speedReset;

	int fovUp;
	int fovDown;
	int fovReset;

	int teleportHome;
	int toggleInertia;
	int toggleWireframe; //w, most likely the same as forward, but this will check if control is also currently held
	int toggleDev; //control D heh

	int togglePause;
	int menuConfirm; //"enter"
	int menuUp;
	int menuDown;
	int menuLeft;
	int menuRight;
	//most likely more controls in the future, doh
};

//uses tinyxml2 to load and parse settings xml files needed, can and will be expanded upon the need of extra functionality
class Settings
{
private:
	GraphicsSettings graphicsSettings;
	UserSettings userSettings;

public:
	Settings();
	~Settings();

	int setup();

	//called during runtime if needed
	template<typename T>
	int writeGraphicsSettings(const char *element, T value);

	inline const GraphicsSettings &getGraphicsSettings()
	{
		return this->graphicsSettings;
	}

	template<typename T>
	int writeUserSettings(std::string element, T value);

	inline const UserSettings &getUserSettings()
	{
		return this->userSettings;
	}

protected:
	int loadFile(tinyxml2::XMLDocument &doc, const char *filePath, const char *defaults);
	void checkElement(tinyxml2::XMLElement *element, const char *filePath);

	//setup() should load these internally
	int loadGraphicsSettings();

	int loadUserSettings();

};

#endif


