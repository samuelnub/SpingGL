#include <game/game.h>
#include <render/renderable.h>
#include <SDL2/SDL.h>

Game::Game()
{
	//setup other things, gui, fonts, etc.
	window.setup();
	states.setup();
	input.setup(this);
	commands.setup(this);

	meshes.setup();
	shaders.setup();
	textures.setup();

	eyes.setup();
	scene.setup(this);

	std::string testText = "test";
	std::vector<std::string> _shadeTest;
	_shadeTest.push_back(testText);
	std::map<std::string, std::vector<std::string>> _templol;
	_templol[testText].push_back(testText);

	glm::mat4 testMat;
	testMat = glm::translate(testMat, glm::vec3(0.0f, 0.0f, 0.0f));
	testoRendo.create(69, testMat, &this->meshes, &this->shaders, &this->textures, _shadeTest, _templol);
	scene.stage(&testoRendo);

	while (true)
		this->loop();
}

Game::~Game()
{
}

void Game::loop()
{
	input.update(SDL_GetTicks());

	if (this->states.get("nevada") == -1)
	{
		this->states.add("nevada", true);
	}

	testoRendo.update(glm::rotate(*testoRendo.getTrans(), glm::radians(0.05f * this->input.getDelta()), glm::vec3(0.5f, 0.5f, 0.0f)));

	scene.draw();

	SDL_GL_SwapWindow(this->window._window);
}