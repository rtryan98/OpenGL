#include "Util.h"

#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <fstream>

#include <glad/glad.h>

#ifdef _MSC_VER
#define COLORS
#endif

namespace Log
{
	void write(const char* prefix, const char* message, va_list args)
	{
		vprintf((std::string(prefix) + message + '\n').c_str(), args);
	}

	void writeColored(const char* colorcode, const char* prefix, const char* message, va_list args)
	{
		vprintf((std::string(colorcode) + prefix + message + '\n').c_str(), args);
		printf("\u001b[0m");
	}

	void trace(const char* message, ...)
	{
		va_list args;
		va_start(args, message);
#ifdef COLORS
		writeColored("\u001b[90m", "[TRACE]: ", message, args);
#else
		write("[TRACE]: ", message, args);
#endif
		va_end(args);
	}

	void info(const char* message, ...)
	{
		va_list args;
		va_start(args, message);
		write("[INFO]: ", message, args);
		va_end(args);
	}

	void warn(const char* message, ...)
	{
		va_list args;
		va_start(args, message);
#ifdef COLORS
		writeColored("\u001b[33m", "[WARN]: ", message, args);
#else
		write("[WARN]: ", message, args);
#endif
		va_end(args);
	}

	void error(const char* message, ...)
	{
		va_list args;
		va_start(args, message);
#ifdef COLORS
		writeColored("\u001b[91m", "[ERROR]: ", message, args);
#else
		write("[ERROR]: ", message, args);
#endif
		va_end(args);
	}

	void fatal(const char* message, ...)
	{
		va_list args;
		va_start(args, message);
#ifdef COLORS
		writeColored("\u001b[31m", "[FATAL]: ", message, args);
#else
		write("[FATAL]: ", message, args);
#endif
		va_end(args);
		exit(1);
	}
}

namespace File
{
	const char* fileToString(const char* path)
	{
		static std::string result;
		result.clear();
		std::ifstream input;
		input.open(path);
		if (input.fail())
		{
			Log::error("Error opening file: %s", path);
			return result.c_str();
		}
		std::string line;
		while (getline(input, line))
		{
			result.append(line + '\n');
		}
		input.close();
		return result.c_str();
	}
}

namespace OpenGL
{
	void messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
	{
		const auto sourceString = [source]()
		{
			switch (source)
			{
			case GL_DEBUG_SOURCE_API:				return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY:		return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION:		return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER:				return "OTHER";
			default:								return "NONE";
			}
		}();

		const auto typeString = [type]()
		{
			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:				return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY:			return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE:			return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER:				return "MARKER";
			case GL_DEBUG_TYPE_OTHER:				return "OTHER";
			case GL_DEBUG_TYPE_POP_GROUP:			return "POP_GROUP";
			case GL_DEBUG_TYPE_PUSH_GROUP:			return "PUSH_GROUP";
			default:								return "NONE";
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

	void initDebugCallback()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(messageCallback, nullptr);
		// uncommenting this will disable notifications from the Debug Callback
//		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	}
}
