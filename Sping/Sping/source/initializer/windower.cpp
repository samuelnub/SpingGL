#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <initializer/windower.h>
#include <tinyxml2/tinyxml2.h>
#include <iostream>
#include <globals/xmldefaults.h>

Window::Window()
{
}

Window::~Window()
{
	SDL_GL_DeleteContext(this->_context);
	SDL_Quit();
}

void Window::setup()
{
	this->_failedAttempts = 0;
	this->loadXML("res/settings/settings.xml");

	SDL_Init(SDL_INIT_VIDEO);

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, this->_glMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, this->_glMinor);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, this->_stencilBits);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, this->_multisamples);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, this->_doublebuffer);

	if (SDL_GetDesktopDisplayMode(0, &this->_displayMode) != 0)
	{
		std::cout << "Couldn't get desktop display mode for some reason!\n" << SDL_GetError() << "\n";
		exit(EXIT_FAILURE);
	}

	const char *tempName = this->_name.c_str();

	if (this->_fullscreen == 1)
	{
		this->_window = SDL_CreateWindow(tempName, 0, 0, this->_displayMode.w, this->_displayMode.h,
			SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
			);
	}
	else
	{
		this->_window = SDL_CreateWindow(tempName, 100, 100, this->_width, this->_height,
			SDL_WINDOW_OPENGL
			);
	}
	

	if (this->_window == NULL)
	{
		throw "Couldn't create SDL OpenGL window!\n";
		exit(EXIT_FAILURE);
	}

	/*
	SDL_WINDOW_FULLSCREEN
	fullscreen window
	SDL_WINDOW_FULLSCREEN_DESKTOP
	fullscreen window at the current desktop resolution
	SDL_WINDOW_OPENGL
	window usable with OpenGL context
	SDL_WINDOW_HIDDEN
	window is not visible
	SDL_WINDOW_BORDERLESS
	no window decoration
	SDL_WINDOW_RESIZABLE
	window can be resized
	SDL_WINDOW_MINIMIZED
	window is minimized
	SDL_WINDOW_MAXIMIZED
	window is maximized
	SDL_WINDOW_INPUT_GRABBED
	window has grabbed input focus
	SDL_WINDOW_ALLOW_HIGHDPI
	window should be created in high-DPI mode if supported (>= SDL 2.0.1)
	*/

	this->_context = SDL_GL_CreateContext(this->_window);

	glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK)
	{
		throw "Couldn't init GLEW!\n";
	}

	glViewport(0, 0, this->_width, this->_height);
}

void Window::loadXML(const char * filePath)
{
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLError xmlErr = xmlDoc.LoadFile(filePath);
	if (xmlErr != tinyxml2::XML_SUCCESS)
	{
		this->_failedAttempts++;
		std::cout << "Couldn't read XML file for window settings! Making defaults...\n";


		xmlDoc.Parse(XMLDefaults::defaultSettings);
		xmlDoc.SaveFile(filePath);

		if (this->_failedAttempts < 69)
			this->loadXML(filePath);
		else
			exit(EXIT_FAILURE);
	}

	//TODO: actual interpretation of settings.xml
	tinyxml2::XMLElement *rootNode = xmlDoc.FirstChildElement("root");
	rootNode = rootNode->FirstChildElement("WindowSettings");
	rootNode->FirstChildElement("glMajor")->QueryIntText(&this->_glMajor);
	rootNode->FirstChildElement("glMinor")->QueryIntText(&this->_glMinor);

	rootNode->FirstChildElement("depthBits")->QueryIntText(&this->_depthBits);
	rootNode->FirstChildElement("stencilBits")->QueryIntText(&this->_stencilBits);
	rootNode->FirstChildElement("doubleBuffer")->QueryBoolText(&this->_doublebuffer);
	rootNode->FirstChildElement("multisamples")->QueryIntText(&this->_multisamples);
	this->_name = rootNode->FirstChildElement("title")->FirstChild()->ToText()->Value();
	std::cout << rootNode->FirstChildElement("title")->FirstChild()->ToText()->Value() << "\n";

	rootNode->FirstChildElement("width")->QueryIntText(&this->_width);
	rootNode->FirstChildElement("height")->QueryIntText(&this->_height);
	rootNode->FirstChildElement("fullscreen")->QueryBoolText(&this->_fullscreen);
	if (rootNode == nullptr)
	{
		std::cout << "Couldn't read a single element from XML file for settings! Try deleting the file at " << filePath << " and run this again to generate a new settings.xml file!\n";
		exit(EXIT_FAILURE);
	}
}
