#include <render/scene.h>
#include <game/game.h>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <tools/meshes.h>
#include <tools/shaders.h>
#include <tools/textures.h>

#include <SDL2/SDL_opengl.h>

#include <boost/uuid/uuid_io.hpp>

Scene::Scene()
{
}

Scene::~Scene()
{
}

int Scene::setup(Game *game)
{
	this->_stagedRenderables.reserve(100000);
	//this->_staged.resize(static_cast<int>(DrawPriority::ENUM_COUNT));

	this->_gamePtr = game;

	std::cout << "Set up the scene object!\n";
	return 0;
}

int Scene::stage(Renderable *renderable)
{
	if (std::find(this->_stagedRenderables.begin(), this->_stagedRenderables.end(), renderable) != this->_stagedRenderables.end())
	{
		std::cout << "The renderable with an address of" << renderable << " is already staged!\n";
		return -1;
	}
	this->_stagedRenderables.push_back(renderable);

	for (auto &shader : *renderable->getShaderPtrs())
	{
		this->_staged[static_cast<int>(renderable->getDrawPriority())][shader].insert(
			this->_staged[static_cast<int>(renderable->getDrawPriority())][shader].end(),
			renderable
			);
	}
	std::cout << "Successfully staged the renderable with an address of " << renderable << " with a UUID of " << boost::lexical_cast<std::string>(renderable->getUUID()) << "\n";

	return 0;
}

int Scene::unstage(Renderable * renderable)
{
	if (std::find(this->_stagedRenderables.begin(), this->_stagedRenderables.end(), renderable) == this->_stagedRenderables.end())
	{
		//doesnt exist, not staged in the first place
		return -1;
	}

	this->_stagedRenderables.erase(std::remove(this->_stagedRenderables.begin(), this->_stagedRenderables.end(), renderable), this->_stagedRenderables.end());
	
	for (auto &shader : *renderable->getShaderPtrs())
	{
		this->_staged.at(static_cast<int>(renderable->getDrawPriority()))[shader].erase(std::remove(
			this->_staged.at(static_cast<int>(renderable->getDrawPriority()))[shader].begin(),
			this->_staged.at(static_cast<int>(renderable->getDrawPriority()))[shader].end(),
			renderable
			),
			this->_staged.at(static_cast<int>(renderable->getDrawPriority()))[shader].end());
	}
	std::cout << "Successfully unstaged the renderable with an address of " << renderable << " with a UUID of " << boost::lexical_cast<std::string>(renderable->getUUID()) << "\n";

	return 0;
}

bool Scene::isStaged(Renderable *renderable)
{
	return std::find(this->_stagedRenderables.begin(), this->_stagedRenderables.end(), renderable) != this->_stagedRenderables.end();
}

//an important function, i think, dunno, kinda draws everything to your screen
void Scene::draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	for (auto &drawPriority : this->_staged)
	{
		glClear(GL_DEPTH_BUFFER_BIT); //NEEDS THIS FOR DRAW PRIORITY TO ACTUALLY MEAN ANYTHING

		for (auto &shader : drawPriority)
		{
			this->_gamePtr->shaders.utilise(shader.first);

			//TODO: The player's camera class should have a map with draw priority as key and view matrices as elements
			glUniformMatrix4fv(glGetUniformLocation(shader.first->programID, "view"), 1, GL_FALSE, glm::value_ptr(this->_gamePtr->actormanager.player._camera.getViewMat()));
			glUniformMatrix4fv(glGetUniformLocation(shader.first->programID, "projection"), 1, GL_FALSE, glm::value_ptr(this->_gamePtr->actormanager.player._camera.getProjMat()));

			for (auto &renderable : shader.second)
			{
				glUniformMatrix4fv(glGetUniformLocation(shader.first->programID, "model"), 1, GL_FALSE, glm::value_ptr(*renderable->getTrans()));
				
				for (auto &mesh : *renderable->getMeshTexPtrs())
				{
					GLuint diffCount = 1;
					GLuint specCount = 1;
					GLuint normCount = 1;

					for (unsigned int i = 0; i < mesh.second.size(); i++)
					{
						glActiveTexture(GL_TEXTURE0 + i);
						std::stringstream ss;
						std::string number;
						std::string name = mesh.second.at(i)->type;

						if (name == "texture_diffuse")
						{
							ss << diffCount++;
						}
						else if (name == "texture_specular")
						{
							ss << specCount++;
						}
						else if (name == "texture_normal")
						{
							ss << normCount++;
						}
						number = ss.str();

						glUniform1f(glGetUniformLocation(shader.first->programID, ("material." + name + number).c_str()), i);
						glBindTexture(GL_TEXTURE_2D, mesh.second.at(i)->texID);
					}

					glBindVertexArray(mesh.first->vaoID);
					switch (renderable->getDrawType())
					{
					case DrawType::TRIANGLES:
						glDrawElements(GL_TRIANGLES, this->_gamePtr->meshes.getIndexData(mesh.first->pushID)->size(), GL_UNSIGNED_INT, 0);
						break;
					case DrawType::LINES:
						glDrawElements(GL_LINES, this->_gamePtr->meshes.getIndexData(mesh.first->pushID)->size(), GL_UNSIGNED_INT, 0);
						break;
					case DrawType::POINTS:
						glDrawElements(GL_POINTS, this->_gamePtr->meshes.getIndexData(mesh.first->pushID)->size(), GL_UNSIGNED_INT, 0);
						break;
					default:
						std::cout << "how did you not specify a draw type???! i cant believe you've done this\n";
						break;
					}
					glBindVertexArray(0);

					for (unsigned int i = 0; i < mesh.second.size(); i++)
					{
						glActiveTexture(GL_TEXTURE0 + i);
						glBindTexture(GL_TEXTURE_2D, 0);
					}
				}
			}
		}
	}
}

//TODO: implement flushing
void Scene::flush()
{

}