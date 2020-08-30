#include "Util.h"

#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <fstream>

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
