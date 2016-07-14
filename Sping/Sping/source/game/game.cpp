#include <game/game.h>
#include <render/renderable.h>
#include <SDL2/SDL.h>

Game::Game()
{
	settings.setup();
	window.setup(this);
	states.setup();
	input.setup(this);
	commands.setup(this);

	meshes.setup();
	shaders.setup();
	textures.setup();

	scene.setup(this);

	//TODO: stupid test stuff lol
	textures.store("you_tried", "res/textures/test.jpg", "texture_diffuse");

	std::string testText = "test";
	std::string testTextTwo = "you_tried";
	std::vector<std::string> _shadeTest;
	_shadeTest.push_back(testText);
	std::map<std::string, std::vector<std::string>> _templol;
	_templol[testText].push_back(testText);
	std::map<std::string, std::vector<std::string>> _templolTwo;
	_templolTwo[testText].push_back(testTextTwo);

	glm::mat4 testMat;
	testMat = glm::translate(testMat, glm::vec3(0.0f, 0.0f, 0.0f));
	testoRendo.create(69, testMat, &this->meshes, &this->shaders, &this->textures, _shadeTest, _templol);

	testoRendoTwo.create(42, testMat, &this->meshes, &this->shaders, &this->textures, _shadeTest, _templolTwo);
	scene.stage(&testoRendo);
	scene.stage(&testoRendoTwo);

	camera.setup(glm::radians(60.0f), 1280.0f / 720.0f, 0.001f, 1000000.0f, glm::vec3(0.0f, 0.0f, 1.0f));

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