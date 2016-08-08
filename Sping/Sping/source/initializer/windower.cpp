#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <initializer/windower.h>
#include <iostream>
#include <game/game.h>

Window::Window()
{
}

Window::~Window()
{
	SDL_GL_DeleteContext(this->_context);
	SDL_Quit();
}

void Window::setup(Game *game)
{
	this->_gamePtr = game;

	SDL_Init(SDL_INIT_VIDEO);

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, this->_gamePtr->settings.getGraphicsSettings().glMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, this->_gamePtr->settings.getGraphicsSettings().glMinor);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, this->_gamePtr->settings.getGraphicsSettings().stencilBits);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, this->_gamePtr->settings.getGraphicsSettings().multisamples);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, this->_gamePtr->settings.getGraphicsSettings().doublebuffer);

	if (SDL_GetDesktopDisplayMode(0, &this->_displayMode) != 0)
	{
		std::cout << "Couldn't get desktop display mode for some reason!\n" << SDL_GetError() << "\n";
		exit(EXIT_FAILURE);
	}

	const char *name = this->_gamePtr->settings.getGraphicsSettings().name.c_str();

	if (this->_gamePtr->settings.getGraphicsSettings().fullscreen == 1)
	{
		this->_window = SDL_CreateWindow(name, 0, 0, this->_displayMode.w, this->_displayMode.h,
			SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
			);
	}
	else
	{
		this->_window = SDL_CreateWindow(name, 100, 100, this->_gamePtr->settings.getGraphicsSettings().width, this->_gamePtr->settings.getGraphicsSettings().height,
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
	
	this->_gottenRenderer = SDL_GetRenderer(this->_window);
	SDL_GetRendererOutputSize(this->_gottenRenderer, &this->createdWidth, &this->createdHeight);

	glViewport(0, 0, this->createdWidth, this->createdHeight);
}