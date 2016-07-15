#pragma once
#ifndef TOOLS_CAMERA_H
#define TOOLS_CAMERA_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <SDL2/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//full credit due to dustin-biser on github, https://github.com/dustin-biser/Rigid3D/blob/master/src/Rigid3D/Graphics/Camera.hpp
//a camera class, which controls the view and projection matrices. this is not the "player" class, it simply provides ways to transform the player's eyes
class Camera
{
private:
	float _fovY;
	float _aspect;
	float _nearPlane;
	float _farPlane;
	bool _recalcProjMat;
	glm::mat4 _projMat;
	unsigned int _floatTolerance;

	glm::vec3 _eyePos;
	glm::quat _orientation;

	glm::vec3 _leftVec;
	glm::vec3 _upVec;
	glm::vec3 _frontVec;

	bool _recalcViewMat;
	glm::mat4 _viewMat;
	//normalize vectors once this has been exceeded, or else cumulated rotations will lose precision
	unsigned int _rotHitCount;
	unsigned int _rotHitCountMax;

public:
	Camera();
	~Camera();
	//TODO: read preferences for stuff like fov from xml file
	void setup(float fovY, float aspect, float nearPlane, float farPlane, const glm::vec3 &spawnPos);

	//setters
	void setFOV(float newFOV);
	void setAspect(float newAspect);
	void setNearPlane(float newNearPlane);
	void setFarPlane(float newFarPlane);

	void setPos(float x, float y, float z);
	void setPos(const glm::vec3 &vec);

	//getters
	inline float getFOV()
	{
		return this->_fovY;
	}

	inline float getAspect()
	{
		return this->_aspect;
	}

	inline float getNearPlane()
	{
		return this->_nearPlane;
	}

	inline float getFarPlane()
	{
		return this->_farPlane;
	}

	inline glm::vec3 getPos()
	{
		return this->_eyePos;
	}

	inline glm::vec3 getLeftDir()
	{
		return this->_leftVec;
	}

	inline glm::vec3 getUpDir()
	{
		return this->_upVec;
	}

	inline glm::vec3 getFrontDir()
	{
		return this->_frontVec;
	}

	inline glm::quat getOrientation()
	{
		return this->_orientation;
	}

	inline glm::mat4 getViewMat()
	{
		if (this->_recalcViewMat)
		{
			//calc inverse rot
			glm::quat q = this->_orientation;
			q.x *= -1.0f;
			q.y *= -1.0f;
			q.z *= -1.0f;
			this->_viewMat = glm::mat4_cast(q);

			glm::vec3 v = -this->_eyePos;
			glm::mat4 m = this->_viewMat;
			//bottom row
			this->_viewMat[3] = (m[0] * v[0]) + (m[1] * v[1]) + (m[2] * v[2]) + m[3];

			this->_recalcViewMat = false;
		}
		return this->_viewMat;
	}

	inline glm::mat4 getProjMat()
	{
		if (this->_recalcProjMat)
		{
			this->_projMat = glm::perspective(glm::radians(this->_fovY), this->_aspect, this->_nearPlane, this->_farPlane);
			this->_recalcProjMat = false;
		}
		return this->_projMat;
	}

	//actions
	void lookAt(const glm::vec3 &targetVec);
	void lookAt(float targetX, float targetY, float targetZ);
	void lookAt(const glm::vec3 &eyePos, const glm::vec3 &targetVec, const glm::vec3 &upVec);
	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);
	void rotate(float angle, const glm::vec3 &axis);

	void translate(float x, float y, float z);
	void translate(const glm::vec3 &amount);
	void translateLocal(float left, float up, float forward);
	void translateLocal(const glm::vec3 &amount);

protected:
	void initLocalCoordSys(const glm::vec3 &spawnPos);
	void registerRot();
	void normalizeCam();

};


#endif