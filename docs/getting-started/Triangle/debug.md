---
sort: 2
---

# Debugging OpenGL
OpenGL can be overwhelming at first, and if we make an error in most cases we won't see anything on screen. To see what's going wrong we'll setup a minimal logger and a debug callback.
This will allow us to see the error codes and a description of what the error is.

## A simple Logger
When writing a Logger, we usually have different severities. We'll use 5 severities: *trace*, *info*, *warn*, *error* and *fatal*, with *fatal* exiting the program.
Because everything should be able to access the Logger, we'll be placing those functions into a namespace.
```cpp
namespace Log
{
	void trace(const char* message, ... );
	void info(const char* message, ...);
	void warn(const char* message, ...);
	void error(const char* message, ... );
	void fatal(const char* message, ... );
}
```
Now that we have those functions, we can implement them. In my case I've used two implementations: One without color codes and one with them.
For simplicity I'll only show the former here. To use variadic arguments we'll need to include the `stdarg.h` header. For printing those we'll need the `stdio.h` header.
```cpp
#include <stdarg.h>
#include <stdio.h>
#include <string>

namespace Log
{
	void write(const char* prefix, const char* message, va_list args)
	{
		vprintf((std::string(prefix) + message + '\n').c_str(), args);
	}

	void trace(const char* message, ...)
	{
		va_list args;
		va_start(args, message);
		write("[TRACE]: ", message, args);
		va_end(args);
	}

    // implement other severities by copy-pasting the trace implementation and changing the prefix argument
```

## The Debug Callback
OpenGL introduced a method for creating a custom callback which we will use now.
It has the following signature:
```cpp
void messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
```
Inside of this method we can pretty much handle everything how we want. A very brief explaination on the parameters:
* **source** is which "system" caused the error
* **type** is the type of the error, such as performance notifications or deprecated behaviour
* **id** is the ID of the error, such as 1282, which stands for *invalid operation*
* **severity** is the severity
* **length** is the length of the message string
* **message** is the message
* **user_param** those are user defined parameters we could use

For now, the only parameters that really interest us are **source**, **type** and **severity**.
An example implementation could look like this:
```cpp
void messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	const auto sourceString = [source]()
	{
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		default: return "NONE";
		}
	}();
	const auto typeString = [type]()
	{
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		case GL_DEBUG_TYPE_POP_GROUP: return "POP_GROUP";
		case GL_DEBUG_TYPE_PUSH_GROUP: return "PUSH_GROUP";
		default: return "NONE";
		}
	}();
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		Log::trace("OpenGL Debug\n\tSource: %s\n\tType: %s\n\tMessage: %s", sourceString, typeString, message);
		break;
	case GL_DEBUG_SEVERITY_LOW:
		Log::info("OpenGL Debug\n\tSource: %s\n\tType: %s\n\tMessage: %s", sourceString, typeString, message);
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		Log::warn("OpenGL Debug\n\tSource: %s\n\tType: %s\n\tMessage: %s", sourceString, typeString, message);
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		Log::error("OpenGL Debug\n\tSource: %s\n\tType: %s\n\tMessage: %s", sourceString, typeString, message);
		break;
	default:
		Log::info("OpenGL Debug\n\tSource: %s\n\tType: %s\n\tMessage: %s", sourceString, typeString, message);
		break;
	}
}
```
Now all we need to do is, after initializing OpenGL, initialize the Debug Callback with those 4 lines of code:
```cpp
glEnable(GL_DEBUG_OUTPUT);
glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
glDebugMessageCallback(messageCallback, nullptr);
// commenting this will enable notification-level messages from the Debug Callback
glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
```

### Source
The source code of the Logger can be found here: [Util.cpp](https://github.com/rtryan98/OpenGL/blob/master/base/src/Util.cpp) [Util.h](https://github.com/rtryan98/OpenGL/blob/master/base/src/Util.h)
