#pragma once
#ifndef GLOBALS_XMLDEFAULTS_H
#define GLOBALS_XMLDEFAULTS_H

//settings how-to:
//1. know what'cha wanna add, what category does it go to? graphical? user prefs?
//2. add the new element here for when new settings files have to be generated
//3. go to settings.h and add the appropriate variable to the appropriate struct
//4. go to settings.cpp and scroll through that hideously long list of stuff and add your element to be read
//5. go to that actual .xml file in res/settings/.. generated right now and add the element manually
//shouldve automated settings to read and write to a map instead lol, TOO FAR TO GO BACK
namespace XMLDefaults
{
	const char *graphicsSettings =
	{
		"<?xml version=\"1.0\"?>"
		"<root>"
		"  <GraphicsSettings>"
		"    <width>1280</width>"
		"    <height>720</height>"
		"    <fullscreen>0</fullscreen>"
		"	"
		"    <glMajor>3</glMajor>"
		"    <glMinor>3</glMinor>"
		"	"
		"    <depthBits>24</depthBits>"
		"    <stencilBits>8</stencilBits>"
		"    <doublebuffer>1</doublebuffer>"
		"    <multisamples>8</multisamples>"
		"	"
		"    <name>Sping!</name>"
		"	"
		"    <maxActors>1000000</maxActors>"
		"    <maxTexWidth>2048</maxTexWidth>"
		"    <maxTexHeight>2048</maxTexHeight>"
		"    <maxLod>7</maxLod>"
		""
		"  </GraphicsSettings>"
		"</root>"
	};

	const char *userSettings =
	{
		"<?xml version=\"1.0\"?>"
		"<root>"
		"  <UserSettings>"
		"    <zNear>0.0000001</zNear>"
		"    <zFar>1000000000.0</zFar>"
		"    <fov>75.0</fov>"
		"	"
		"    <mouseLookSensitivity>1.0</mouseLookSensitivity>"
		"    <rollSensitivity>0.01</rollSensitivity>"
		"	"
		"    <!--Reference https://wiki.libsdl.org/SDLScancodeLookup if you want to tinker here-->"
		"    <forward>26</forward>"
		"    <backward>22</backward>"
		"    <strafeLeft>4</strafeLeft>"
		"    <strafeRight>7</strafeRight>"
		"    <upward>21</upward>"
		"    <downward>9</downward>"
		"    <rollLeft>20</rollLeft>"
		"    <rollRight>8</rollRight>"
		"	"
		"    <orientationReset>18</orientationReset>"
		"	"
		"    <speedUp>23</speedUp>"
		"    <speedDown>10</speedDown>"
		"    <speedReset>25</speedReset>"
		"	"
		"    <fovUp>75</fovUp>"
		"    <fovDown>78</fovDown>"
		"    <fovReset>74</fovReset>"
		"    <!--These few may seem to overlap other keys, but they will need Ctrl to also be pressed-->"
		"    <teleportHome>11</teleportHome>"
		"    <toggleInertia>12</toggleInertia>"
		"    <toggleWireframe>26</toggleWireframe>"
		"    <toggleDev>7</toggleDev>"
		"	"
		"    <togglePause>41</togglePause>"
		"    <menuConfirm>40</menuConfirm>"
		"    <menuUp>82</menuUp>"
		"    <menuDown>81</menuDown>"
		"    <menuLeft>80</menuLeft>"
		"    <menuRight>79</menuRight>"
		"  </UserSettings>"
		"</root>"
	};


}

#endif