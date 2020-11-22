#include "Camera.h"

#include "Window.h"
#include "Util.h"

#include <glm/glm.hpp>

Camera::~Camera()
{

}

void Camera::init(const float fov, const glm::vec3& position, const float movespeed, const float sensitivity)
{
    this->position = position;
    this->movespeed = movespeed;
    this->sensitivity = sensitivity;
    this->fov = fov;
    front = { 1.0f, 0.0f, 0.0f };
    up = { 0.0f, 1.0f, 0.0f };
    updateVectors();
}

constexpr glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };

void Camera::update(const Window& window, const float dt)
{
    const float speed = movespeed * dt;
    if (window.isKeyPressed(GLFW_KEY_W) && window.isKeyPressed(GLFW_KEY_S))
    {
        // skip
    }
    else if (window.isKeyPressed(GLFW_KEY_W))
    {
        position += speed * front;
    }
    else if (window.isKeyPressed(GLFW_KEY_S))
    {
        position -= speed * front;
    }
    if (window.isKeyPressed(GLFW_KEY_A) && window.isKeyPressed(GLFW_KEY_D))
    {
        // skip
    }
    else if (window.isKeyPressed(GLFW_KEY_A))
    {
        position -= speed * right;
    }
    else if (window.isKeyPressed(GLFW_KEY_D))
    {
        position += speed * right;
    }
    if (window.isKeyPressed(GLFW_KEY_SPACE))
    {
        position += speed * up;
    }
    else if (window.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
        position -= speed * up;
    }

    const glm::dvec2 cursorPos = window.getMousePosition();
    if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
    {
        float xOffset{(float) cursorPos.x - lastX};
        float yOffset{lastY - (float) cursorPos.y};
        xOffset *= sensitivity;
        yOffset *= sensitivity;
        yaw += xOffset;
        pitch += yOffset;
        if (pitch > 89.9f)
        {
            pitch = 89.9f;
        }
        if (pitch < -89.9f)
        {
            pitch  = -89.9f;
        }
    }

    updateVectors();

    lastX = (float) cursorPos.x;
    lastY = (float) cursorPos.y;
}

const glm::vec3& Camera::getPosition() const
{
    return position;
}

const glm::vec3& Camera::getUp() const
{
    return up;
}

const glm::vec3& Camera::getFront() const
{
    return front;
}

const glm::vec3& Camera::getRight() const
{
    return right;
}

const float Camera::getFov() const
{
    return fov;
}

void Camera::updateVectors()
{
    glm::vec3 frontInternal{};
    frontInternal.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    frontInternal.y = glm::sin(glm::radians(pitch));
    frontInternal.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front = glm::normalize(frontInternal);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
