#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <initializer/windower.h>
#include <globals/globals.h>
#include <game/game.h>

Window::Window()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Windowsettings::glMajor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, Windowsettings::glMinor);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, Windowsettings::stencilBits);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, Windowsettings::multisamples);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, Windowsettings::doublebuffer);

	this->_window = SDL_CreateWindow(Windowsettings::name, 0, 0, Windowsettings::width, Windowsettings::height, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP
		);

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

	Game game(this->_window);
}

Window::~Window()
{
	SDL_GL_DeleteContext(this->_context);
	SDL_Quit();
}
