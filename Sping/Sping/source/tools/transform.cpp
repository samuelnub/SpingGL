#include <tools/transform.h>
#include <game/game.h>

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
	const glm::tvec3<double, glm::precision::highp> &scaleReal,
	const glm::tvec3<double, glm::precision::highp> &leftVec, 
	const glm::tvec3<double, glm::precision::highp> &upVec, 
	const glm::tvec3<double, glm::precision::highp> &frontVec, 
	const float &angle, 
	const glm::vec3 &axis)
{
	this->_gamePtr = game;

	this->setDrawPriority(drawPriority);
	this->setWarpBack(warpBack);
	this->setupThreshold();

	this->setPosReal(posReal);
	this->setPosGraphical(glm::vec3(0.0f, 0.0f, 0.0f)); //always; graphically; starts out zeroed

	this->setScaleReal(scaleReal);

	this->_leftVec = leftVec;
	this->_upVec = upVec;
	this->_frontVec = frontVec;

	this->rotate(angle, axis);

	this->_recalcMat = false;

	this->normalize();
}

void Transform::setPosReal(const Vec3Acc & posReal)
{
	this->_posReal = posReal;
	
	this->setPosGraphical(glm::vec3(
		(posReal.x * this->_scaleFactor).ToFloat(), 
		(posReal.y * this->_scaleFactor).ToFloat(),
		(posReal.z * this->_scaleFactor).ToFloat()
		));
}

void Transform::setPosGraphical(const glm::tvec3<double, glm::precision::highp> & posGraphical)
{
	this->_posGraphical = posGraphical;

	this->checkThreshold();

	this->_recalcMat = true;
}

void Transform::addPosReal(const glm::tvec3<double, glm::precision::highp> & posReal)
{
	this->_posRealAccumulated += posReal;

	this->addPosGraphical(
			posReal * this->_scaleFactor
		);

	this->checkAccumulation();
}

void Transform::addPosGraphical(const glm::tvec3<double, glm::precision::highp> & posGraphical)
{
	this->_posGraphical += posGraphical;

	this->checkThreshold();

	this->_recalcMat = true;
}

void Transform::addPosLocalReal(const glm::tvec3<double, glm::precision::highp> & posReal)
{
	this->_posRealAccumulated += posReal.x * this->_leftVec;
	this->_posRealAccumulated += posReal.y * this->_upVec;
	this->_posRealAccumulated += posReal.z * this->_frontVec;

	this->addPosLocalGraphical(
			posReal * this->_scaleFactor
		);

	this->checkAccumulation();
}

void Transform::addPosLocalGraphical(const glm::tvec3<double, glm::precision::highp> & posGraphical)
{
	this->_posGraphical += posGraphical.x * this->_leftVec;
	this->_posGraphical += posGraphical.y * this->_upVec;
	this->_posGraphical += posGraphical.z * this->_frontVec;

	this->checkThreshold();

	this->_recalcMat = true;
}

void Transform::setScaleReal(const glm::tvec3<double, glm::precision::highp> & scaleReal)
{
	//dont really need this, if you want the "real scale", just divide the graphical scale by scale factor
	//this->_scaleReal = scaleReal;

	this->setScaleGraphical(
		scaleReal * this->_scaleFactor
		);
}

void Transform::setScaleGraphical(const glm::tvec3<double, glm::precision::highp> & scaleGraphical)
{
	this->_scaleGraphical = scaleGraphical;

	this->_recalcMat = true;
}

void Transform::roll(double angle)
{
	glm::tquat<double, glm::precision::highp> q = glm::angleAxis(angle, this->_frontVec);

	this->_upVec = glm::rotate(q, this->_upVec);
	this->_leftVec = glm::rotate(q, this->_leftVec);

	this->_orientation = q * this->_orientation;

	this->normalize();
	this->_recalcMat = true;
}

void Transform::pitch(double angle)
{
	glm::tquat<double, glm::precision::highp> q = glm::angleAxis(angle, this->_leftVec);

	this->_upVec = glm::rotate(q, this->_upVec);
	this->_frontVec = glm::rotate(q, this->_frontVec);

	this->_orientation = q * this->_orientation;

	this->normalize();
	this->_recalcMat = true;
}

void Transform::yaw(double angle)
{
	glm::tquat<double, glm::precision::highp> q = glm::angleAxis(angle, this->_upVec);

	this->_leftVec = glm::rotate(q, this->_leftVec);
	this->_frontVec = glm::rotate(q, this->_frontVec);

	this->_orientation = q * this->_orientation;

	this->normalize();
	this->_recalcMat = true;
}

void Transform::rotate(double angle, const glm::tvec3<double, glm::precision::highp> & axis)
{
	glm::tvec3<double, glm::precision::highp> n = glm::normalize(axis);
	glm::tquat<double, glm::precision::highp> q = glm::angleAxis(angle, n);

	this->_leftVec = glm::rotate(q, this->_leftVec);
	this->_upVec = glm::rotate(q, this->_upVec);
	this->_frontVec = glm::rotate(q, this->_frontVec);

	this->_orientation = q * this->_orientation;

	this->normalize();
	this->_recalcMat = true;
}

void Transform::setDrawPriority(DrawPriority drawPriority)
{
	this->_drawPriority = drawPriority;
	this->updateScale();
}

void Transform::setWarpBack(bool warpBack)
{
	this->_posWarpBack - warpBack;
}

const Vec3Acc & Transform::getPosReal()
{
	this->checkAccumulation(true);

	return this->_posReal;
}

const glm::tvec3<double, glm::precision::highp>& Transform::getPosGraphical()
{
	return this->_posGraphical;
}

DrawPriority Transform::getDrawPriority()
{
	return this->_drawPriority;
}

bool Transform::isBeyondThreshold()
{
	return this->_isBeyondThreshold;
}

bool Transform::warpsBack()
{
	return this->_posWarpBack;
}

glm::tvec3<double, glm::precision::highp> Transform::getScaleReal()
{
	return this->_scaleGraphical * (1.0 / this->_scaleFactor);
}

const glm::tvec3<double, glm::precision::highp>& Transform::getScaleGraphical()
{
	return this->_scaleGraphical;
}

const glm::tvec3<double, glm::precision::highp>& Transform::getLeftVec()
{
	return this->_leftVec;
}

const glm::tvec3<double, glm::precision::highp>& Transform::getUpVec()
{
	return this->_upVec;
}

const glm::tvec3<double, glm::precision::highp>& Transform::getFrontVec()
{
	return this->_frontVec;
}

const glm::tquat<double, glm::precision::highp>& Transform::getOrientation()
{
	return this->_orientation;
}

const glm::mat4 & Transform::getMat()
{
	if (this->_recalcMat)
	{
		this->recalcMat();
	}
	return this->_transMat;
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

	this->_updateRealLength = this->_scaleFactor * 5.0;

	this->_scaleFactor = 1.0 / this->_scaleFactor; //save a bit of calculations later
}

void Transform::checkAccumulation(bool forceReset)
{
	if ((forceReset) || (glm::length(this->_posRealAccumulated) > this->_updateRealLength))
	{
		this->_posReal = this->_posReal + this->_posRealAccumulated;
		this->_posRealAccumulated = glm::tvec3<double, glm::precision::highp>();
	}
}

void Transform::checkThreshold()
{
	if (!this->_posWarpBack && (glm::length(this->_posGraphical) > this->_posThresholdLength))
	{
		this->_isBeyondThreshold = true;
	}
	else if (this->_posWarpBack && (glm::length(this->_posGraphical) > this->_posThresholdLength))
	{
		this->_posGraphical = glm::vec3(0.0f, 0.0f, 0.0f);
		this->_isBeyondThreshold = false;
	}
	else
	{
		this->_isBeyondThreshold = false;
	}
}

void Transform::normalize()
{
	this->_leftVec = glm::normalize(this->_leftVec);
	this->_upVec = glm::normalize(this->_upVec);
	this->_frontVec = glm::normalize(this->_frontVec);

	this->_orientation = glm::normalize(this->_orientation);

	this->_leftVec = glm::cross(this->_upVec, this->_frontVec);
	this->_upVec = glm::cross(this->_frontVec, this->_leftVec);
}

void Transform::recalcMat()
{
	glm::tquat<double, glm::precision::highp> q = this->_orientation;

	q.x *= -1.0;
	q.y *= -1.0;
	q.z *= -1.0;
	this->_transMat = glm::mat4_cast(q);

	glm::tvec3<double, glm::precision::highp> v = -this->_posGraphical;
	glm::dmat4 m = this->_transMat;

	this->_transMat[3] = (m[0] * v[0]) + (m[1] * v[1]) + (m[2] * v[2]) + m[3];

	this->_recalcMat = false;
}
