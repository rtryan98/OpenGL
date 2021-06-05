#include "Window.h"

#include "Util.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

Window::Window(const unsigned int width, const unsigned int height, const char* title)
    : width(width), height(height), title(title)
{

}

Window::~Window()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
}

const unsigned int Window::getWidth() const
{
    return width;
}

void Window::setWidth(const unsigned int width)
{
    this->width = width;
    glfwSetWindowSize(handle, width, height);
}

const unsigned int Window::getHeight() const
{
    return height;
}

void Window::setHeight(const unsigned int height)
{
    this->height = height;
    glfwSetWindowSize(handle, width, height);
}

const char* Window::getTitle() const
{
    return title;
}

void Window::setTitle(const char* title)
{
    this->title = title;
    glfwSetWindowTitle(handle, title);
}

GLFWwindow* Window::getNativeHandle()
{
    return handle;
}

void Window::render() const
{
    glfwSwapBuffers(handle);
    glfwPollEvents();
}

void Window::preInit(const unsigned int major, const unsigned int minor)
{
    if (!glfwInit())
    {
        glfwTerminate();
        Log::fatal("GLFW initialization failed!");
        std::abort();
    }
    Log::trace("GLFW initialized.");
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void Window::create()
{
    handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

void Window::postInit()
{
    glfwMakeContextCurrent(handle);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        glfwTerminate();
        Log::fatal("GLAD initialization failed!");
    }
    Log::trace("GLAD initialized.");
}

const bool Window::isCloseRequested() const
{
    return glfwWindowShouldClose(handle);
}

void Window::close()
{
    glfwSetWindowShouldClose(handle, true);
}

const bool Window::isKeyPressed(const unsigned int keycode) const
{
    return glfwGetKey(handle, keycode);
}

const bool Window::isMouseButtonPressed(const unsigned int mouseButton) const
{
    return glfwGetMouseButton(handle, mouseButton);
}

const glm::dvec2 Window::getMousePosition() const
{
    double x{}, y{};
    glfwGetCursorPos(handle, &x, &y);
    return glm::dvec2{x, y};
}
