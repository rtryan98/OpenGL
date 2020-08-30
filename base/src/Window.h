#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	explicit Window(const unsigned int width, const unsigned int height, const char* title);
	Window(Window& other) = delete;
	Window(Window&& other) = delete;
	~Window();
	const unsigned int getWidth() const;
	void setWidth(const unsigned int width);
	const unsigned int getHeight() const;
	void setHeight(const unsigned int height);
	const char* getTitle() const;
	void setTitle(const char* title);
	const GLFWwindow* getNativeHandle() const;
	void render() const;
	void preInit(const unsigned int major, const unsigned int minor);
	void create();
	void postInit();
	const bool isCloseRequested() const;
	void close();
private:
	const char* title;
	unsigned int width;
	unsigned int height;
	GLFWwindow* handle = nullptr;
};
