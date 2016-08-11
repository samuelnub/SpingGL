#include <tools/transform.h>
#include <game/game.h>
#include <render/renderable.h>

Transform::Transform()
{
}

Transform::~Transform()
{
}

Transform::Transform(
	Game *game,
	DrawPriority drawPriority, 
	const Vec3Acc &posReal, 
	bool warpBack, 
	const Vec3Acc &scaleReal, 
	const glm::vec3 &leftVec, 
	const glm::vec3 &upVec, 
	const glm::vec3 &frontVec, 
	const float &angle, 
	const glm::vec3 &axis)
{
	this->_gamePtr = game;

	this->setDrawPriority(drawPriority);
	this->setWarpBack(warpBack);
	this->setupThreshold();

	this->setPosReal(posReal);
	this->setPosGraphical(glm::vec3(0.0f, 0.0f, 0.0f)); //always; graphically; starts out zeroed

	this->_leftVec = leftVec;
	this->_upVec = upVec;
	this->_frontVec = frontVec;

	this->rotate(angle, axis);
}

void Transform::setPosReal(const Vec3Acc & posReal)
{
	this->_posReal = posReal;
	
	this->setPosGraphical(glm::vec3(
		(posReal.x * (1.0 / this->_scaleFactor)).ToFloat(), 
		(posReal.y * (1.0 / this->_scaleFactor)).ToFloat(),
		(posReal.z * (1.0 / this->_scaleFactor)).ToFloat()
		));
}

void Transform::setPosGraphical(const glm::vec3 & posGraphical)
{
	this->_posGraphical = posGraphical;

	this->checkThreshold();
}

void Transform::addPosReal(const glm::dvec3 & posReal)
{
	this->_posRealAccumulated += posReal;

	this->addPosGraphical(
			posReal * (1.0 / this->_scaleFactor)
		);

	this->checkAccumulation();
}

void Transform::addPosReal(const glm::vec3 & posReal)
{
	this->addPosReal(posReal);
}

void Transform::addPosGraphical(const glm::vec3 & posGraphical)
{
	this->_posGraphical += posGraphical;

	this->checkThreshold();
}

void Transform::setDrawPriority(DrawPriority drawPriority)
{
	this->_drawPriority = drawPriority;
	this->updateScale();
}

void Transform::setupThreshold()
{
	this->_posThresholdLength = this->_gamePtr->settings.get("GameGraphicalSize")->_float * this->_gamePtr->settings.get("GameGraphicalThresholdMultiplier")->_int;
	this->checkThreshold();
}

void Transform::updateScale()
{
	switch (this->_drawPriority)
	{
	case DrawPriority::UNIVERSE:
		this->_scaleFactor = this->_gamePtr->settings.get("GameUniverseScale")->_double;
		break;
	case DrawPriority::GALACTIC:
		this->_scaleFactor = this->_gamePtr->settings.get("GameGalacticScale")->_double;
		break;
	case DrawPriority::STELLAR:
		this->_scaleFactor = this->_gamePtr->settings.get("GameStellarScale")->_double;
		break;
	case DrawPriority::PLANETARY:
		this->_scaleFactor = this->_gamePtr->settings.get("GamePlanetaryScale")->_double;
		break;
	case DrawPriority::SURFACE:
		this->_scaleFactor = this->_gamePtr->settings.get("GameSurfaceScale")->_double;
		break;
	}

	this->_updateRealLength = this->_scaleFactor;
}

void Transform::checkAccumulation(bool forceReset)
{
	if ((forceReset) || (glm::length(this->_posRealAccumulated) > this->_updateRealLength))
	{
		this->_posReal = this->_posReal + this->_posRealAccumulated;
		this->_posRealAccumulated = glm::dvec3();
	}
}

void Transform::checkThreshold()
{
	if ((glm::length(this->_posGraphical) > this->_posThresholdLength) && !this->_posWarpBack)
	{
		this->_isBeyondThreshold = true;
	}
	else if ((glm::length(this->_posGraphical) > this->_posThresholdLength) && this->_posWarpBack)
	{
		this->_posGraphical = glm::vec3(0.0f, 0.0f, 0.0f);
		this->_isBeyondThreshold = false;
	}
	else
	{
		this->_isBeyondThreshold = false;
	}
}
