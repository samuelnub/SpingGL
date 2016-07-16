#include <render/scene.h>
#include <game/game.h>
#include <iostream>
#include <render/renderable.h>
#include <algorithm>
#include <sstream>

#include <tools/meshes.h>
#include <tools/shaders.h>
#include <tools/textures.h>

#include <SDL2/SDL_opengl.h>

Scene::Scene()
{
}

Scene::~Scene()
{
}

int Scene::setup(Game *game)
{
	this->_stagedRenderables.reserve(100000);

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
	
	std::vector<Shader *> tempShaders = *renderable->getShaderPtrs();
	for (unsigned int i = 0; i < tempShaders.size(); i++)
	{
		if (tempShaders[i] == nullptr)
		{
			std::cout << "Could not stage renderable with address " << renderable << " which requests for a shader at address " << tempShaders[i] << "!\a\n";
			return -1;
		}
		this->_stagedShaders[tempShaders[i]].push_back(renderable);
	}

	this->_stagedRenderables.push_back(renderable);
	std::cout << "Successfully staged renderable with address " << this->_stagedRenderables.back() << "!\n";
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

	std::map<Shader *, std::vector<Renderable *>>::iterator iter;
	iter = this->_stagedShaders.begin();
	while (iter != this->_stagedShaders.end())
	{
		/*if (std::find(iter->second.begin(), iter->second.end(), renderable) == iter->second.end())
		{
			continue;
		}*/
		iter->second.erase(std::remove(iter->second.begin(), iter->second.end(), renderable), iter->second.end());
		iter++;
	}

	std::cout << "Unstaged renderable with address " << renderable << "!\n";

	return 0;
}

bool Scene::isStaged(Renderable *renderable)
{
	return std::find(this->_stagedRenderables.begin(), this->_stagedRenderables.end(), renderable) != this->_stagedRenderables.end();
}

//an important function, i think, dunno, kinda draws everything to your screen
void Scene::draw()
{
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	//go through each shader
	std::map<Shader *, std::vector<Renderable *>>::const_iterator iter;
	iter = this->_stagedShaders.begin();
	while (iter != this->_stagedShaders.end())
	{
		//use shader, send shader-specific uniforms
		this->_gamePtr->shaders.utilise(iter->first);

		//TODO: sending camera matrices should go here (V and P)

		glUniformMatrix4fv(glGetUniformLocation(iter->first->programID, "view"), 1, GL_FALSE, glm::value_ptr(this->_gamePtr->actormanager.player._camera.getViewMat()));
		glUniformMatrix4fv(glGetUniformLocation(iter->first->programID, "projection"), 1, GL_FALSE, glm::value_ptr(this->_gamePtr->actormanager.player._camera.getProjMat()));

		//==========================================================
		//=========Each renderable that uses this shader============
		//==========================================================
		//render each renderable that depends on this current shader iteration, make sure your shader has the naming convention containing "model"
		for (unsigned int i = 0; i < iter->second.size(); i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(iter->first->programID, "model"), 1, GL_FALSE, glm::value_ptr(*iter->second[i]->getTrans()));

			//==========================================================
			//==========Each mesh that this renderable uses=============
			//==========================================================
			std::map<Mesh *, std::vector<Texture *>> *tempMeshTexes = iter->second[i]->getMeshTexPtrs();
			for (auto &element : *tempMeshTexes)
			{
				//==========================================================
				//============Each texture that this mesh uses==============
				//==========================================================
				GLuint diffCount = 1;
				GLuint specCount = 1;
				GLuint normCount = 1;
				for (unsigned int j = 0; j < element.second.size(); j++)
				{
					glActiveTexture(GL_TEXTURE0 + j);
					std::stringstream ss;
					std::string number;
					std::string name = element.second.at(j)->type;

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

					glUniform1f(glGetUniformLocation(iter->first->programID, ("material." + name + number).c_str()), j);
					glBindTexture(GL_TEXTURE_2D, element.second[j]->texID);
				}

				glBindVertexArray(element.first->vaoID);
				switch (iter->second[i]->getDrawType())
				{
				case DrawType::TRIANGLES:
					glDrawElements(GL_TRIANGLES, this->_gamePtr->meshes.getIndexData(element.first->pushID)->size(), GL_UNSIGNED_INT, 0);
					break;
				case DrawType::LINES:
					glDrawElements(GL_LINES, this->_gamePtr->meshes.getIndexData(element.first->pushID)->size(), GL_UNSIGNED_INT, 0);
					break;
				case DrawType::POINTS:
					glDrawElements(GL_POINTS, this->_gamePtr->meshes.getIndexData(element.first->pushID)->size(), GL_UNSIGNED_INT, 0);
					break;
				default:
					std::cout << "how did you not specify a draw type???! i cant believe you've done this\n";
					break;
				}
				glBindVertexArray(0);

				for (unsigned int j = 0; j < element.second.size(); j++)
				{
					glActiveTexture(GL_TEXTURE0 + j);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			//Still in one renderable, looping through meshes
		}
		iter++;
	}


}

//TODO: implement flushing
void Scene::flush()
{

}