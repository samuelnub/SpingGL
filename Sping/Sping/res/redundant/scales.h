#pragma once
#ifndef TOOLS_SCALES_H
#define TOOLS_SCALES_H

#include <render/renderable.h>
#include <ttmath/ttmath.h>
#include <array>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Game;

//the numbers are the "word size", on 32 bit, its 32 bits  per word, on 64 bits, its; you guessed it, 64 bits, or you can explicitly demand the bits you want, you monster
struct Vec3Big
{
	ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> x;
	ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> y;
	ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> z;

	Vec3Big()
	{
		this->x = "0.0";
		this->y = "0.0";
		this->z = "0.0"; //you can give it a const char or a double (eg 0.0 without quotes), dunno the perfomance difference
	}

	Vec3Big(const std::string &xStr, const std::string &yStr, const std::string &zStr)
	{
		this->x = xStr.c_str;
		this->y = yStr.c_str;
		this->z = zStr.c_str;
	}

	Vec3Big(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

//handles transformations of different scales, only scaling and translation are needed, rotation is the same no matter the scale
//pretty much just a helper class, but im keeping the stingy act of "not having static classes"
class Scales
{
private:
	Game *_gamePtr = nullptr;

	std::array<ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)>, static_cast<int>(DrawPriority::ENUM_COUNT)> _scaleFactors;

	ttmath::Big<TTMATH_BITS(128), TTMATH_BITS(256)> _one;

public:
	Scales();
	~Scales();

	int setup(Game *game);

	//give me your vector that you want to transform very precisely, give me the real amount you want (1:1) and i'll do it scaled down with the scale you want
	inline void translate(Vec3Big &vecBig, const Vec3Big &realAmount, DrawPriority scaleFactor)
	{
		// multiply by 1/scale to avoid div by 0
		vecBig.x += (realAmount.x * (this->_one / this->_scaleFactors[static_cast<int>(scaleFactor)]));
		vecBig.y += (realAmount.y * (this->_one / this->_scaleFactors[static_cast<int>(scaleFactor)]));
		vecBig.z += (realAmount.z * (this->_one / this->_scaleFactors[static_cast<int>(scaleFactor)]));
	}

	inline void scale(Vec3Big &vecBig, const Vec3Big &realAmount, DrawPriority scaleFactor)
	{
		vecBig.x *= (realAmount.x * (this->_one / this->_scaleFactors[static_cast<int>(scaleFactor)]));
		vecBig.y *= (realAmount.y * (this->_one / this->_scaleFactors[static_cast<int>(scaleFactor)]));
		vecBig.z *= (realAmount.z * (this->_one / this->_scaleFactors[static_cast<int>(scaleFactor)]));
	}

	//give me your "accurate vector", and i'll convert it to a float-based glm vec3 so you can send it to opengl
	inline glm::vec3 downsample(const Vec3Big &vecBig)
	{
		return glm::vec3(vecBig.x.ToFloat(), vecBig.y.ToFloat(), vecBig.z.ToFloat());
	}

protected:


};

#endif