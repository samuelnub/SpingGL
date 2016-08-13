#include <tools/camera.h>

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::setup(float fovY, float aspect, float nearPlane, float farPlane, const glm::vec3 &spawnPos)
{
	this->_rotHitCountMax = 10000;
	this->_rotHitCount = 0;
	this->_recalcViewMat = true;

	this->_fovY = fovY;
	this->_aspect = aspect;
	this->_nearPlane = nearPlane;
	this->_farPlane = farPlane;
	this->_floatTolerance = 5;
	this->_projMat = glm::perspective(
		glm::radians(this->_fovY),
		this->_aspect,
		this->_nearPlane,
		this->_farPlane);
	this->_recalcProjMat = false;

	this->initLocalCoordSys(spawnPos);

	this->normalizeCam();
}

void Camera::setFOV(float newFOV)
{
	if (newFOV < 0.00001f) //yes, i'd like one 0 degree fov please
	{
		newFOV = 0.00001f;
	}
	else if (newFOV > 180.0f)
	{
		newFOV = 180.0f;
	}

	this->_fovY = newFOV;
	this->_recalcProjMat = true;
}

void Camera::setAspect(float newAspect)
{
	this->_aspect = newAspect;
	this->_recalcProjMat = true;
}

void Camera::setNearPlane(float newNearPlane)
{
	this->_nearPlane = newNearPlane;
	this->_recalcProjMat = true;
}

void Camera::setFarPlane(float newFarPlane)
{
	this->_farPlane = newFarPlane;
	this->_recalcProjMat = true;
}

void Camera::setPos(float x, float y, float z)
{
	this->_eyePos.x = x;
	this->_eyePos.y = y;
	this->_eyePos.z = z;

	this->_recalcViewMat = true;
}

void Camera::setPos(const glm::vec3 &vec)
{
	this->setPos(vec.x, vec.y, vec.z); //hehe
}

void Camera::initLocalCoordSys(const glm::vec3 &spawnPos)
{
	this->_leftVec.x = -1.0f;
	this->_leftVec.y = 0.0f;
	this->_leftVec.z = 0.0f;

	this->_upVec.x = 0.0f;
	this->_upVec.y = 1.0f;
	this->_upVec.z = 0.0f;

	this->_frontVec.x = 0.0f;
	this->_frontVec.y = 0.0f;
	this->_frontVec.z = 1.0f;

	this->_eyePos.x = spawnPos.x;
	this->_eyePos.y = spawnPos.y;
	this->_eyePos.z = spawnPos.z;
}

void Camera::registerRot()
{
	this->_rotHitCount++;

	if (this->_rotHitCount > this->_rotHitCountMax)
	{
		this->_rotHitCount = 0;
		this->normalizeCam();
	}
}

void Camera::normalizeCam()
{
	this->_leftVec = glm::normalize(this->_leftVec);
	this->_upVec = glm::normalize(this->_upVec);
	this->_frontVec = glm::normalize(this->_frontVec);

	this->_orientation = glm::normalize(this->_orientation);

	this->_leftVec = glm::cross(this->_upVec, this->_frontVec);
	this->_upVec = glm::cross(this->_frontVec, this->_leftVec);
}

void Camera::lookAt(const glm::vec3 & targetVec)
{
	this->lookAt(targetVec.x, targetVec.y, targetVec.z);
}

void Camera::lookAt(float targetX, float targetY, float targetZ)
{
	this->_frontVec.x = targetX - this->_eyePos.x;
	this->_frontVec.y = targetY - this->_eyePos.y;
	this->_frontVec.z = targetZ - this->_eyePos.z;
	this->_frontVec = glm::normalize(this->_frontVec);

	//TODO: front could equal up
	{
		this->_leftVec = this->_eyePos + this->_upVec;
		float t = -1.0f * glm::dot(this->_frontVec, this->_upVec);

		this->_leftVec.x += t * this->_frontVec.x;
		this->_leftVec.y += t * this->_frontVec.y;
		this->_leftVec.z += t * this->_frontVec.z;

		this->_upVec = this->_leftVec - this->_eyePos;
		this->_upVec = glm::normalize(this->_upVec);
	}

	this->_leftVec = glm::cross(this->_upVec, this->_frontVec);

	//if front and up are not ortho anymore, correct it
	if (glm::dot(this->_frontVec, this->_upVec) > 0.0000001f)
	{
		this->_upVec = glm::cross(this->_frontVec, this->_upVec);
	}

	glm::mat3 m;
	m[0] = -1.0f * this->_leftVec; //cam's local x axis
	m[1] = this->_upVec; //cam's local y axis
	m[2] = -1.0f * this->_frontVec; //cam's local z axis
	this->_orientation = glm::toQuat(m);

	this->registerRot();
	this->_recalcViewMat = true;
}

void Camera::lookAt(const glm::vec3 & eyePos, const glm::vec3 & targetVec, const glm::vec3 & upVec)
{
	this->_eyePos = eyePos;

	this->_frontVec = glm::normalize(targetVec - eyePos);
	this->_leftVec = glm::normalize(glm::cross(this->_upVec, this->_frontVec));
	this->_upVec = glm::cross(this->_frontVec, this->_leftVec);

	glm::mat3 m;
	m[0] = -1.0f * this->_leftVec; //cam's local x axis
	m[1] = this->_upVec; //cam's local y axis
	m[2] = -1.0f * this->_frontVec; //cam's local z axis
	this->_orientation = glm::quat_cast(m);

	this->registerRot();
	this->_recalcViewMat = true;
}

void Camera::roll(float angle)
{
	glm::quat q = glm::angleAxis(angle, this->_frontVec);

	this->_upVec = glm::rotate(q, this->_upVec);
	this->_leftVec = glm::rotate(q, this->_leftVec);

	this->_orientation = q * this->_orientation;

	this->registerRot();
	this->_recalcViewMat = true;
}

void Camera::pitch(float angle)
{
	//TODO: try'n fix this
	glm::quat q = glm::angleAxis(angle, this->_leftVec);

	this->_upVec = glm::rotate(q, this->_upVec);
	this->_frontVec = glm::rotate(q, this->_frontVec);

	this->_orientation = q * this->_orientation;

	this->registerRot();
	this->_recalcViewMat = true;
}

void Camera::yaw(float angle)
{
	glm::quat q = glm::angleAxis(angle, this->_upVec);

	this->_leftVec = glm::rotate(q, this->_leftVec);
	this->_frontVec = glm::rotate(q, this->_frontVec);

	this->_orientation = q * this->_orientation;

	this->registerRot();
	this->_recalcViewMat = true;
}

void Camera::rotate(float angle, const glm::vec3 & axis)
{
	glm::vec3 n = glm::normalize(axis);
	glm::quat q = glm::angleAxis(angle, n);

	this->_leftVec = glm::rotate(q, this->_leftVec);
	this->_upVec = glm::rotate(q, this->_upVec);
	this->_frontVec = glm::rotate(q, this->_frontVec);

	this->_orientation = q * this->_orientation;

	this->registerRot();
	this->_recalcViewMat = true;
}

void Camera::translate(float x, float y, float z)
{
	this->_eyePos.x += x;
	this->_eyePos.y += y;
	this->_eyePos.z += z;

	this->_recalcViewMat = true;
}

void Camera::translate(const glm::vec3 & amount)
{
	this->translate(amount.x, amount.y, amount.z);
}

void Camera::translateLocal(float left, float up, float forward)
{
	this->_eyePos += left * this->_leftVec;
	this->_eyePos += up * this->_upVec;
	this->_eyePos += forward * this->_frontVec;

	this->_recalcViewMat = true;
}

void Camera::translateLocal(const glm::vec3 & amount)
{
	this->translateLocal(amount.x, amount.y, amount.z);
}
