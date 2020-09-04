#pragma once

#include <unordered_map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
	explicit Shader();
	Shader(const Shader& other) = delete;
	Shader(const Shader&& other) = delete;
	~Shader();
	void create();
	void addShader(const char* path, const GLenum type);
	void compile();
	void bind() const;
	void unbind() const;
	void setUniform4f(const std::string& name, const float x, const float y, const float z, const float w) const;
	void setUniform3f(const std::string& name, const float x, const float y, const float z) const;
	void setUniform2f(const std::string& name, const float x, const float y) const;
	void setUniform1f(const std::string& name, const float val) const;
	void setUniform1i(const std::string& name, const int val) const;
	void setUniformmat4f(const std::string& name, const glm::mat4& val) const;
	void dispatch(const unsigned int sizeX, const unsigned int sizeY, const unsigned int sizeZ) const;
private:
	GLint getUniformLocation(const std::string& name) const;
private:
	unsigned int id;
	mutable std::unordered_map<std::string, GLint> uniformLocationCache;
};
