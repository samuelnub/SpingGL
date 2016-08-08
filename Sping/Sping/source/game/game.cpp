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
	
	uuidgen.setup();

	actormanager.setup(this);
	scene.setup(this);

	std::string testText = "test";
	std::vector<std::string> _shadeTest;
	_shadeTest.push_back(testText);
	std::map<std::string, std::vector<std::string>> _templol;
	_templol[testText].push_back(testText);

	glm::mat4 testMat;
	testMat = glm::translate(testMat, glm::vec3(0.0f, 0.0f, 0.0f));
	testoRendo.create(testMat, this, _shadeTest, _templol, DrawType::TRIANGLES, DrawPriority::PLANETARY);

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

	testoRendo.update(glm::rotate(*testoRendo.getTrans(), glm::radians(0.05f * this->input.getDelta()), glm::vec3(0.0f, -1.0f, 0.0f)));

	glm::mat4 testMat;
	testMat = glm::rotate(*testoRendo.getTrans(), 0.000001f * (this->input.getDelta()/100.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	actormanager.update();
	scene.draw();

	SDL_GL_SwapWindow(this->window._window);
}