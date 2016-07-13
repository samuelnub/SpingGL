#pragma once
#ifndef RENDER_CAMERAS_EYES_H
#define RENDER_CAMERAS_EYES_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

//credit to http://hamelot.io/visualization/moderngl-camera/

#include <SDL2/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum EyeMovement
{
	UP,
	DOWN,
	STRAFE_LEFT,
	STRAFE_RIGHT,
	FORWARD,
	BACKWARD,
	ROLL_LEFT, //TODO: implement rolling
	ROLL_RIGHT
};

//this is not the game's "player" class, it is simply a class encapsulating the transformation of the view and projection matrices. the real player class should compose of this, and take input calls and then call these.
class Eyes
{
private:
	double _aspect;
	double _FOV;
	double _nearPlane;
	double _farPlane;

	float _eyeScale;
	float _eyeHeading;
	float _eyePitch;

	float _maxPitchRate;
	float _maxHeadingRate;

	glm::vec3 _eyePos;
	glm::vec3 _eyePosDelta;
	glm::vec3 _eyeLookAt;
	glm::vec3 _eyeDirection;

	glm::vec3 _eyeUp;
	glm::quat _rotQuat;
	glm::vec3 _mousePos;

	glm::mat4 _modelMat;
	glm::mat4 _viewMat;
	glm::mat4 _projMat;
	glm::mat4 _mvp;

public:
	Eyes();
	~Eyes();

	void setup();

	void reset();
	//updates the camera matrices, and subsequently the position etc.
	void update();

	void move(EyeMovement movement);

	void changePitch(float deg);

	void changeHeading(float deg);
	//change heading and pitch based on mouse movement and delta
	void move2D(int x, int y);

	void setPosition(glm::vec3 pos);

	void setLookAt(glm::vec3 pos);

	void setFOV(double fov);

	void setPlanes(double nearPlane, double farPlane);

	void setDist(double eyeDist);

	void setPos(int x, int y);
	
	//getters
	void getMatrices(glm::mat4 &v, glm::mat4 &p);

protected:


};

#endif