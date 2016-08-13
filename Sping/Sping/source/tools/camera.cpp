#include <tools/camera.h>
#include <game/game.h>

Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera::Camera(Game *game, Transform & viewTransform, float fov, float aspect, float nearPlane, float farPlane)
{
	this->_gamePtr = game;

	this->setupTransforms(viewTransform);

	this->_recalcProjMat = false;

	this->setFOV(fov);
	this->setAspect(aspect);
	this->setNearPlane(nearPlane);
	this->setFarPlane(farPlane);
}

void Camera::setFOV(float fov)
{
	this->_fovY = fov;
	this->_recalcProjMat = true;
}

void Camera::setAspect(float aspect)
{
	this->_aspect = aspect;
	this->_recalcProjMat = true;
}

void Camera::setNearPlane(float nearPlane)
{
	this->_nearPlane = nearPlane;
	this->_recalcProjMat = true;
}

void Camera::setFarPlane(float farPlane)
{
	this->_farPlane = farPlane;
	this->_recalcProjMat = true;
}

void Camera::move(const glm::tvec3<double, glm::precision::highp>& amount)
{
	for (auto &view : this->_views)
	{
		view.addPosLocalReal(amount);
	}
}

void Camera::roll(double angle)
{
	for (auto &view : this->_views)
	{
		view.roll(angle);
	}
}

void Camera::pitch(double angle)
{
	for (auto &view : this->_views)
	{
		view.pitch(angle);
	}
}

void Camera::yaw(double angle)
{
	for (auto &view : this->_views)
	{
		view.yaw(angle);
	}
}

const Vec3Acc & Camera::getPosReal(DrawPriority index)
{
	return this->_views[static_cast<int>(index)].getPosReal();
}

const glm::tvec3<double, glm::precision::highp>& Camera::getPosGraphical(DrawPriority index)
{
	return this->_views[static_cast<int>(index)].getPosGraphical();
}

const glm::mat4 & Camera::getViewMat(DrawPriority index)
{
	return this->_views[static_cast<int>(index)].getMat();
}

const glm::mat4 & Camera::getProjMat()
{
	if (this->_recalcProjMat)
	{
		this->recalcProjMat();
	}
	return this->_projMat;
}

void Camera::setupTransforms(Transform & transform)
{
	for (int i = 0; i < static_cast<int>(DrawPriority::ENUM_COUNT); i++)
	{
		this->_views[i] = Transform(
			this->_gamePtr,
			static_cast<DrawPriority>(i),
			transform.getPosReal(),
			transform.warpsBack(),
			glm::tvec3<double, glm::precision::highp>(0.0,0.0,0.0),
			transform.getLeftVec(),
			transform.getUpVec(),
			transform.getFrontVec()
			);
	}
}

void Camera::recalcProjMat()
{
	this->_projMat = glm::perspective(
		glm::radians(this->_fovY),
		this->_aspect,
		this->_nearPlane,
		this->_farPlane
		);

	this->_recalcProjMat = false;
}

