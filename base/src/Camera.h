#pragma once

#include "Window.h"

#include <glm/glm.hpp>

class Camera
{
public:
	Camera() = default;
	~Camera();
	void init(const float fov, const glm::vec3& position, const float movespeed, const float sensitivity);
	void update(const Window& window, const float dt);
	const glm::vec3& getPosition() const;
	const glm::vec3& getUp() const;
	const glm::vec3& getFront() const;
	const glm::vec3& getRight() const;
	const float getFov() const;
private:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	float fov;
	float movespeed;
	float sensitivity;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
private:
	void updateVectors();
};
