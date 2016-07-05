#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <game/game.h>

Game::Game(SDL_Window *window)
{
	this->_window = window;

	//setup other things, gui, fonts, etc.
	states.setup();
	input.setup();

	this->loop();
}

Game::~Game()
{
}

void Game::loop()
{
	SDL_Event event;
	while (true)
	{
		input.update(SDL_GetTicks());

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(this->_window);
	}
}
