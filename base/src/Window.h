#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
    explicit Window(const unsigned int width, const unsigned int height, const char* title);
    Window(const Window& other) = delete;
    Window(const Window&& other) = delete;
    ~Window();
    const unsigned int getWidth() const;
    void setWidth(const unsigned int width);
    const unsigned int getHeight() const;
    void setHeight(const unsigned int height);
    const char* getTitle() const;
    void setTitle(const char* title);
    GLFWwindow* getNativeHandle();
    void render() const;
    void preInit(const unsigned int major, const unsigned int minor);
    void create();
    void postInit();
    const bool isCloseRequested() const;
    void close();
    const bool isKeyPressed(const unsigned int keycode) const;
    const bool isMouseButtonPressed(const unsigned int mouseButton) const;
    const glm::dvec2 getMousePosition() const;
private:
    const char* title;
    unsigned int width;
    unsigned int height;
    GLFWwindow* handle = nullptr;
};
