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

	galaxyGenerator.setup(this);
	//TODO: stupid test stuff lol
	textures.store("starColor", "res/textures/starColor.bmp", "texture_diffuse");

	std::string testText = "test";
	std::string testTextTwo = "starColor";
	std::vector<std::string> _shadeTest;
	_shadeTest.push_back(testText);
	std::map<std::string, std::vector<std::string>> _templol;
	_templol[testText].push_back(testText);
	std::map<std::string, std::vector<std::string>> _templolTwo;
	_templolTwo[testTextTwo].push_back(testTextTwo);

	glm::mat4 testMat;
	testMat = glm::translate(testMat, glm::vec3(0.0f, 0.0f, 0.0f));
	testoRendo.create(testMat, this, _shadeTest, _templol);
	//TODO: if the number is too small, the max count will be rounded to 0, and when making a mesh, if its 0, itll crash at runtime lol, so check if the vertex and index data is empty when you store a mesh
	//same seeded module and same
	galaxyGenerator.generate(426969997);

	Galaxy *tempGal = galaxyGenerator.get(426969997);
	GLuint indexCount = 1;
	std::vector<Vertex> tempVertData;
	std::vector<GLuint> tempIndexData;
	for (unsigned int i = 0; i < tempGal->stars.size(); i++)
	{
		tempVertData.push_back(tempGal->stars.at(i).vertInfo);
		tempIndexData.push_back(indexCount);
		indexCount++;
	}

	meshes.store("starColor", tempVertData, tempIndexData);

	testMat = glm::scale(glm::vec3(2000.0f, 2000.0f, 2000.0f));

	
	testoRendoTwo.create(testMat, this, _shadeTest, _templolTwo, DrawType::POINTS);



	scene.stage(&testoRendo);
	scene.stage(&testoRendoTwo);

	while (true)
		this->loop();
}

Game::~Game()
{
}

void Game::loop()
{
	input.update(SDL_GetTicks());

	testoRendo.update(glm::rotate(*testoRendo.getTrans(), glm::radians(0.05f * this->input.getDelta()), glm::vec3(0.5f, 0.5f, 0.0f)));

	glm::mat4 testMat;
	//testMat = glm::rotate(*testoRendo.getTrans(), glm::radians(0.00001f * this->input.getDelta()), glm::vec3(0.0f, 1.0f, 0.0f));
	testMat = glm::scale(testMat, glm::vec3(2000.0f, 2000.0f, 2000.0f));
	testoRendoTwo.update(testMat);

	actormanager.update();
	scene.draw();

	SDL_GL_SwapWindow(this->window._window);
}