---
sort: 1
---

# Window
The first thing that needs to be done to draw something with OpenGL is that we create a window. There are many libraries out there, but we'll be using GLFW. It's easy to use and fast to setup.
You can also use your platform specific windowing system or another library, but in this tutorial, I won't go over other APIs for windowing and input handling.

## Creating the Window class
To use GLFW, we first have to include it with `#include <GLFW/glfw3.h>`. To create the most basic window, we'll only require 2 functions, namely `glfwInit()` and `glfwCreateWindow()`.
But we want a bit more than the most basic window.
GLFW offers so called *window hints*, which will specify what exactly we want to do with the window, such as making it resizable or even make it invisible to do some offscreen rendering!
To use a window created with GLFW, we'll also need three functions more. Those are `glfwWindowShouldClose()`, `glfwSwapBuffers()` and `glfwPollEvents()`.
They do the following: Ask GLFW if the specified window requested a close, swaps the backbuffer (GLFW is double buffering, so we're drawing to one buffer while the other is shown) and poll things like Input and OS-specific events.
For a basic window, we'll need the following: a width, a height and a title.
GLFW also requires a pointer to the window itself, so in total, our window will have the following members:
* The width and height
* The title
* The pointer to the handle

Which will look like this
```cpp
class Window
{
public:
    // functions etc.
private:
    unsigned int width;
    unsigned int height;
    const char* title;
    GLFWwindow* handle = nullptr;
}
```
Our window constructor will only take in the raw data and store it in the class, the creation will happen in three other functions.
```cpp
Window::Window(const unsigned int width, const unsigned int height, const char* title)
    : width(width), height(height), title(title)
{}
```

### Initializing the Window
We'll initialize the window in three steps: `preInit()`, which will setup GLFW and our default window hints such us the OpenGL version and profile, `create()`, which will create the window and `postInit()`, which will initialize our OpenGL loader.
```cpp
void Window::preInit(const unsigned int versionMajor, const unsigned int versionMinor)
{
    if(!glfwInit())
    {
        Log::fatal("GLFW initialization failed!");
    }
        // this will reset GLFW into its base state
    glfwDefaultWindowHints();
        // sets the major OpenGL Version to major, so major.X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        // sets the minor OpenGL version to minor, so Y.minor
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        // we want to use the OpenGL core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // we don't want deprecated OpenGL functions available to us
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
        // for the ease of this tutorial, we don't want to handle resizing for now
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}
```
Now that we have initialized GLFW, we can continue and create our window!
```cpp
void Window::create()
{
        // create the window
    handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
}
```
We're almost done! Now we only need to create the context and initialize GLAD!
```warning
The program will not compile when you include GLFW before GLAD. Make sure your include order is correct.
```
```cpp
void Window::postInit()
{
    // we need to make the context current before we initialize GLAD
glfwMakeContextCurrent(handle);
    // here we initialize GLAD
if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
{
	glfwTerminate();
	Log::fatal("GLAD initialization failed!");
}
Log::trace("GLAD initialized.");
}
```
### Using the Window
To use our window, we'll simply create three new methods: `render()`, `isCloseRequested()` and `close()`.
```cpp
void Window::render() const
{
    glfwSwapBuffers(handle);
    glfwPollEvents();
}

const bool Window::isCloseRequested() const
{
    return glfwWindowShouldClose(handle);
}

void Window::close()
{
    glfwSetWindowShouldClose(handle, true);
}
```

### Input
To handle input in a very basic way, we'll need three more functions: `isKeyPressed()`, `isMouseButtonPressed()` and `getMousePosition()`. The names should explain what they are doing.
```cpp
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
```
```note
GLFW uses *key codes* and *mouse codes* to find out which key or button is pressed. The glfw3.h header has all the codes available. `GLFW_MOUSE_BUTTON_1` for example is the left click.
```

### Freeing Resources afterwards
As you've seen in the `postInit()` method, there is a call to `glfwTerminate()`, which will basically deinit GLFW. We can modify our destructor to take care of the window destruction for us.
```cpp
Window::~Window()
{
    glfwDestroyWindow(handle);
    glfwTerminate();
}
```
Now we're done with our window class! Of course, GLFW offers much more functionality but for our use cases this will suffice. We can now create a basic renderloop!
```cpp
int main(int argc, char* argv[])
{
    Window window{1600, 900, "Hello Window!"};
    window.preInit(4, 6);
    window.create();
    window.postInit();
    while(!window.isCloseRequested())
    {
        window.render();
        if(window.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            window.close();
        }
    }
    return 0;
}
```
You'll see a black window in the specified size now and it shouldn't crash! If you press Escape on your keyboard, the window should close and the program will terminate!

### Source
You can find the full source code of the window class here: [Window.cpp](https://github.com/rtryan98/OpenGL/blob/master/base/src/Window.cpp) [Window.h](https://github.com/rtryan98/OpenGL/blob/master/base/src/Window.h)

## Additional Resources
* [Official GLFW window guide](https://www.glfw.org/docs/latest/window_guide.html)
