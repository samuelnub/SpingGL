#include <render/cameras/eyes.h>

Eyes::Eyes()
{
}

Eyes::~Eyes()
{
}

void Eyes::setup()
{
	//TODO: read from config
	this->_eyeUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->_FOV = 90.0f;
	this->_rotQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	this->_eyePosDelta = glm::vec3(0.0f, 0.0f, 0.0f);
	this->_eyeScale = 0.5f;
	this->_maxPitchRate = 5;
	this->_maxHeadingRate = 5;
	this->_aspect = double(1280.0f) / double(720.0f);

	this->setPosition(glm::vec3(0.0f, 0.0f, -1.0f));
	this->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
	this->setPlanes(0.01f, 1000.0f);
	this->setFOV(60.0f);
}

void Eyes::reset()
{
	this->_eyeUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Eyes::update()
{
	this->_eyeDirection = glm::normalize(this->_eyeLookAt - this->_eyePos);


	glLoadIdentity();

	this->_projMat = glm::perspective(glm::radians(this->_FOV), this->_aspect, this->_nearPlane, this->_farPlane);

	glm::vec3 axis = glm::cross(this->_eyeDirection, this->_eyeUp);

	glm::quat pitchQuat = glm::angleAxis(this->_eyePitch, axis);

	glm::quat headingQuat = glm::angleAxis(this->_eyeHeading, this->_eyeUp);

	glm::quat temp = glm::cross(pitchQuat, headingQuat);

	temp = glm::normalize(temp);

	this->_eyeDirection = glm::rotate(temp, this->_eyeDirection);

	this->_eyePos += this->_eyePosDelta;

	this->_eyeLookAt = this->_eyePos + this->_eyeDirection * 1.0f;

	//smoothing
	this->_eyePitch *= 0.5f;
	this->_eyeHeading *= 0.5f;
	this->_eyePosDelta = this->_eyePosDelta * 0.5f;

	this->_viewMat = glm::lookAt(this->_eyePos, this->_eyeLookAt, this->_eyeUp);

	//remember to send off your matrix uniforms!
}

void Eyes::move(EyeMovement movement)
{
	switch (movement)
	{
	case UP:
		this->_eyePosDelta += this->_eyeUp * this->_eyeScale;
		break;
	case DOWN:
		this->_eyePosDelta -= this->_eyeUp * this->_eyeScale;
		break;
	case STRAFE_LEFT:
		this->_eyePosDelta -= glm::cross(this->_eyeDirection, this->_eyeUp) * this->_eyeScale;
		break;
	case STRAFE_RIGHT:
		this->_eyePosDelta += glm::cross(this->_eyeDirection, this->_eyeUp) * this->_eyeScale;
		break;
	case FORWARD:
		this->_eyePosDelta += this->_eyeDirection * this->_eyeScale;
		break;
	case BACKWARD:
		this->_eyePosDelta -= this->_eyeDirection * this->_eyeScale;
		break;
	
	}
}

void Eyes::changePitch(float deg)
{
	//so we dont move too fast
	if (deg < -this->_maxPitchRate)
	{
		deg = -this->_maxPitchRate;
	}
	else if (deg > this->_maxPitchRate)
	{
		deg = this->_maxPitchRate;
	}

	this->_eyePitch += deg;

	//check bounds for pitch 
	if (this->_eyePitch > 360.0f)
	{
		this->_eyePitch -= 360.0f;
	}
	else if (this->_eyePitch < -360.0f)
	{
		this->_eyePitch += 360.0f;
	}
}

void Eyes::changeHeading(float deg)
{
	if (deg < -this->_maxHeadingRate)
	{
		deg = -this->_maxHeadingRate;
	}
	else if (deg > this->_maxHeadingRate)
	{
		deg = this->_maxHeadingRate;
	}

	//affected by whether the eyes are looking upward or downward
	if (this->_eyePitch > 90.0f && this->_eyePitch < 270.0f ||
		(this->_eyePitch < -90.0f && this->_eyePitch > -270.0f))
	{
		this->_eyeHeading -= deg;
	}
	else
	{
		this->_eyeHeading += deg;
	}

	if (this->_eyeHeading > 360.0f)
	{
		this->_eyeHeading -= 360.0f;
	}
	else if (this->_eyeHeading < -360.0f)
	{
		this->_eyeHeading += 360.0f;
	}
}

void Eyes::move2D(int x, int y)
{
	glm::vec3 mouseDelta = this->_mousePos - glm::vec3(x, y, 0);
	//TODO: read off settings
	//if cam is moving, aka mouse was clicked and dragged, change pitch and heading
	this->changeHeading(0.08f * mouseDelta.x);
	this->changePitch(0.08f * mouseDelta.y);

	this->_mousePos = glm::vec3(x, y, 0);
}

void Eyes::setPosition(glm::vec3 pos)
{
	this->_eyePos = pos;
}

void Eyes::setLookAt(glm::vec3 pos)
{
	this->_eyeLookAt = pos;
}

void Eyes::setFOV(double fov)
{
	this->_FOV = fov;
}

void Eyes::setPlanes(double nearPlane, double farPlane)
{
	this->_nearPlane = nearPlane;
	this->_farPlane = farPlane;
}

void Eyes::setDist(double eyeDist)
{
	//TODO: ye
}

void Eyes::setPos(int x, int y)
{
	this->_mousePos = glm::vec3(x, y, 0.0f);
}

void Eyes::getMatrices(glm::mat4 & v, glm::mat4 & p)
{
	v = this->_viewMat;
	p = this->_projMat;
}
