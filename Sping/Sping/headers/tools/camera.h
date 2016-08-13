#pragma once
#ifndef TOOLS_CAMERA_H
#define TOOLS_CAMERA_H

#include <tools/transform.h>
#include <array>

class Game;

//full credit due to dustin-biser on github, https://github.com/dustin-biser/Rigid3D/blob/master/src/Rigid3D/Graphics/Camera.hpp
//a camera class, which controls the view and projection matrices. this is not the "player" class, it simply provides ways to transform the player's eyes
class Camera
{
private:
	Game *_gamePtr = nullptr;

	float _fovY;
	float _aspect;
	float _nearPlane;
	float _farPlane;
	
	bool _recalcProjMat;
	glm::mat4 _projMat;

	std::array<Transform, static_cast<int>(DrawPriority::ENUM_COUNT)> _views;

public:
	Camera();
	~Camera();
	
	//ignore the fact that you're passing in a whole transform component, the drawpriority argument doesn't matter, just the others
	Camera(
		Game *game,
		Transform &viewTransform,
		float fov,
		float aspect,
		float nearPlane,
		float farPlane
		);

	void setFOV(float fov);
	void setAspect(float aspect);
	void setNearPlane(float nearPlane);
	void setFarPlane(float farPlane);

	void move(const glm::tvec3<double, glm::precision::highp> &amount);
	void roll(double angle);
	void pitch(double angle);
	void yaw(double angle);

	const Vec3Acc &getPosReal(DrawPriority index = DrawPriority::UNIVERSE);
	const glm::tvec3<double, glm::precision::highp> &getPosGraphical(DrawPriority index);

	const glm::mat4 &getViewMat(DrawPriority index);
	const glm::mat4 &getProjMat();

protected:
	void setupTransforms(Transform &transform);
	void recalcProjMat();

};


#endif