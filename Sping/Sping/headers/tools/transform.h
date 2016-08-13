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

	Vec3Acc operator+(const glm::tvec3<double, glm::precision::highp> &dvec)
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

	DrawPriority _drawPriority;
	double _scaleFactor; //grab from settings
	bool _posWarpBack; // if its true, it will set the graphical pos back to 0,0,0 once it exceeds the threshold
	float _posThresholdLength; // if its pos passes this length, itll either warp back if that option's true, or can be queried by eg. the renderable, and unstage itself. this is calculated by some multiple of the setting's "GameGraphicalSize"
	bool _isBeyondThreshold;

	//dont wanna update the real vecAcc everytime we translate, that'd be costly as heck
	double _updateRealLength;
	glm::tvec3<double, glm::precision::highp> _posRealAccumulated;

	//the 2 aspects which are affected by this scaling down. rotation isnt affected
	glm::tvec3<double, glm::precision::highp> _posGraphical;
	glm::tvec3<double, glm::precision::highp> _scaleGraphical;

	glm::tvec3<double, glm::precision::highp> _leftVec;
	glm::tvec3<double, glm::precision::highp> _upVec;
	glm::tvec3<double, glm::precision::highp> _frontVec;
	glm::tquat<double, glm::precision::highp> _orientation;

	bool _recalcMat;

	glm::mat4 _transMat; //final form, to be sent to glsl. all those "storage variables" above store it as doubles, but translate down (albeit a bit lossily)to this float-based matrix so opengl can use it efficiently

public:
	Transform();
	~Transform();

	//give real pos, store it graphically too
	Transform(
		Game *game,
		DrawPriority drawPriority,
		const Vec3Acc &posReal,
		bool warpBack = false,
		const glm::tvec3<double, glm::precision::highp> &scaleReal = glm::tvec3<double, glm::precision::highp>(1.0, 1.0, 1.0),
		const glm::tvec3<double, glm::precision::highp> &leftVec = glm::tvec3<double, glm::precision::highp>(-1.0f, 0.0f, 0.0f),
		const glm::tvec3<double, glm::precision::highp> &upVec = glm::tvec3<double, glm::precision::highp>(0.0f, 1.0f, 0.0f),
		const glm::tvec3<double, glm::precision::highp> &frontVec = glm::tvec3<double, glm::precision::highp>(0.0f, 0.0f, 1.0f),
		const float &angle = 0.0f,
		const glm::vec3 &axis = glm::vec3(1.0f, 0.0f, 0.0f)
		);
	
	//in reality, you (the external source) should only be calling addPosReal() and addPosLocalReal() instead of setting or adding graphical values
	//setting absolute will not cause the "accumulation vector" to pile up, only adders below
	//won't be using this very often, most likely
	void setPosReal(const Vec3Acc &posReal);
	//won't be using this very often, most likely
	void setPosGraphical(const glm::tvec3<double, glm::precision::highp> &posGraphical);

	void addPosReal(const glm::tvec3<double, glm::precision::highp> &posReal);
	//won't be using this very often, most likely
	void addPosGraphical(const glm::tvec3<double, glm::precision::highp> &posGraphical);
	
	void addPosLocalReal(const glm::tvec3<double, glm::precision::highp> &posReal);
	//won't be using this very often, most likely
	void addPosLocalGraphical(const glm::tvec3<double, glm::precision::highp> &posGraphical);

	void setScaleReal(const glm::tvec3<double, glm::precision::highp> &scaleReal);
	//won't be using this very often, most likely
	void setScaleGraphical(const glm::tvec3<double, glm::precision::highp> &scaleGraphical);

	void roll(double angle);
	void pitch(double angle);
	void yaw(double angle);
	void rotate(double angle, const glm::tvec3<double, glm::precision::highp> &axis);
	//TODO: "lookAt(upVec, targetVec)" implementation
	void setDrawPriority(DrawPriority drawPriority);
	void setWarpBack(bool warpBack);

	//getters
	const Vec3Acc &getPosReal();
	const glm::tvec3<double, glm::precision::highp> &getPosGraphical();
	DrawPriority getDrawPriority();
	bool isBeyondThreshold();
	bool warpsBack();
	glm::tvec3<double, glm::precision::highp> getScaleReal();
	const glm::tvec3<double, glm::precision::highp> &getScaleGraphical();
	const glm::tvec3<double, glm::precision::highp> &getLeftVec();
	const glm::tvec3<double, glm::precision::highp> &getUpVec();
	const glm::tvec3<double, glm::precision::highp> &getFrontVec();
	const glm::tquat<double, glm::precision::highp> &getOrientation();

	const glm::mat4 &getMat();

protected:
	void setupThreshold();
	void updateScale();
	void checkAccumulation(bool forceReset = false); //update real position, as well as reset accumulated glmvec3
	void checkThreshold();
	void normalize();
	void recalcMat();

};

#endif