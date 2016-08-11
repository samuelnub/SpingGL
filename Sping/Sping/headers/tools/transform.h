#pragma once
#ifndef TOOLS_TRANSFORM_H
#define TOOLS_TRANSFORM_H

#include <ttmath/ttmath.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class DrawPriority; //legal in c++11

//accurate vec 3
struct Vec3Acc
{
	ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> x;
	ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> y;
	ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> z;

	Vec3Acc()
	{
		this->x = 0.0;
		this->y = 0.0;
		this->z = 0.0;
	}

	Vec3Acc(const ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> &x, const ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> &y, const ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> &z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3Acc(const std::string &x, const std::string &y, const std::string &z)
	{
		this->x = x.c_str;
		this->y = y.c_str;
		this->z = z.c_str;
	}

	Vec3Acc(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3Acc(const glm::vec3 &xyz)
	{
		this->x = xyz.x;
		this->y = xyz.y;
		this->z = xyz.z;
	}

	Vec3Acc operator+(const Vec3Acc &vecAcc)
	{
		return Vec3Acc(
			(this->x + vecAcc.x),
			(this->y + vecAcc.y),
			(this->z + vecAcc.z)
			);
	}

	Vec3Acc operator+(const glm::vec3 &vec)
	{
		return Vec3Acc(
			(this->x + vec.x),
			(this->y + vec.y),
			(this->z + vec.z)
			);
	}

	Vec3Acc operator+(const glm::dvec3 &dvec)
	{
		return Vec3Acc(
			(this->x + dvec.x),
			(this->y + dvec.y),
			(this->z + dvec.z)
			);
	}

	Vec3Acc operator*(const Vec3Acc &vecAcc)
	{
		return Vec3Acc(
			(this->x * vecAcc.x),
			(this->y * vecAcc.y),
			(this->z * vecAcc.z)
			);
	}

	Vec3Acc operator*(const glm::vec3 &vec)
	{
		return Vec3Acc(
			(this->x * vec.x),
			(this->y * vec.y),
			(this->z * vec.z)
			);
	}

	Vec3Acc operator*(const glm::vec3 &dvec)
	{
		return Vec3Acc(
			(this->x * dvec.x),
			(this->y * dvec.y),
			(this->z * dvec.z)
			);
	}

	bool operator==(const Vec3Acc &vecAcc)
	{
		return
			(this->x == vecAcc.x) && (this->y == vecAcc.y) && (this->z == vecAcc.z);
	}

	bool operator!=(const Vec3Acc &vecAcc)
	{
		return
			!(*this == vecAcc);
	}

	ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> length()
	{
		return ttmath::Sqrt(
			(this->x * this->x) +
			(this->y * this->y) +
			(this->z * this->z)
			);
	}
};

//custom transform class, for one "layer" of scale
class Transform
{
private:
	Game *_gamePtr;

	Vec3Acc _posReal; // real position, pretty large presumably
	Vec3Acc _scaleReal;

	DrawPriority _drawPriority;
	double _scaleFactor; //grab from settings
	bool _posWarpBack; // if its true, it will set the graphical pos back to 0,0,0 once it exceeds the threshold
	float _posThresholdLength; // if its pos passes this length, itll either warp back if that option's true, or can be queried by eg. the renderable, and unstage itself. this is calculated by some multiple of the setting's "GameGraphicalSize"
	bool _isBeyondThreshold;

	//dont wanna update the real vecAcc everytime we translate, that'd be costly as heck
	double _updateRealLength;
	glm::dvec3 _posRealAccumulated;

	//the 2 aspects which are affected by this scaling down. rotation isnt affected
	glm::vec3 _posGraphical;
	glm::vec3 _scaleGraphical;

	glm::vec3 _leftVec;
	glm::vec3 _upVec;
	glm::vec3 _frontVec;
	glm::quat _orientation;

	glm::mat4 _transMat; //final form, to be sent to glsl

public:
	Transform();
	~Transform();

	//give real pos, store it graphically too
	Transform(
		Game *game,
		DrawPriority drawPriority,
		const Vec3Acc &posReal,
		bool warpBack = false,
		const Vec3Acc &scaleReal = Vec3Acc(1.0, 1.0, 1.0),
		const glm::vec3 &leftVec = glm::vec3(-1.0f, 0.0f, 0.0f),
		const glm::vec3 &upVec = glm::vec3(0.0f, 1.0f, 0.0f),
		const glm::vec3 &frontVec = glm::vec3(0.0f, 0.0f, 1.0f),
		const float &angle = 0.0f,
		const glm::vec3 &axis = glm::vec3(1.0f, 0.0f, 0.0f)
		);
	
	//setting absolute will not cause the "accumulation vector" to pile up, only adders below
	//won't be using this very often, most likely
	void setPosReal(const Vec3Acc &posReal);
	//won't be using this very often, most likely
	void setPosGraphical(const glm::vec3 &posGraphical);

	//won't be using this very often, most likely
	void addPosReal(const Vec3Acc &posReal);
	void addPosReal(const glm::vec3 &posReal);
	//won't be using this very often, most likely
	void addPosGraphical(const glm::vec3 &posGraphical);

	//won't be using this very often, most likely
	void addPosLocalReal(const Vec3Acc &posReal);
	void addPosLocalReal(const glm::vec3 &posReal);
	//won't be using this very often, most likely
	void addPosLocalGraphical(const glm::vec3 &posGraphical);

	//won't be using this very often, most likely
	void setScaleReal(const Vec3Acc &scaleReal);
	void setScaleReal(const glm::vec3 &scaleReal);
	//won't be using this very often, most likely
	void setScaleGraphical(const glm::vec3 &scaleGraphical);

	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);
	void rotate(float angle, const glm::vec3 &axis);

	void setDrawPriority(DrawPriority drawPriority);
	void setWarpBack(bool warpBack);

	//getters
	Vec3Acc &getPosReal();

protected:
	void setupThreshold();
	void updateScale();
	void checkAccumulation(bool forceReset = false); //update real position, as well as reset accumulated glmvec3
	void checkThreshold();


};

#endif