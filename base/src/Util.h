#pragma once

namespace Log
{
	void trace(const char* message, ... );
	void info(const char* message, ...);
	void warn(const char* message, ...);
	void error(const char* message, ... );
	void fatal(const char* message, ... );
}

namespace File
{
	const char* fileToString(const char* path);
}

namespace OpenGL
{
	void initDebugCallback();
}
