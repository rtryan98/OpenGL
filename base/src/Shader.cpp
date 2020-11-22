#include "Shader.h"

#include "Util.h"

Shader::Shader()
    : id(0), uniformLocationCache()
{

}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::create()
{
    id = glCreateProgram();
}

void Shader::addShader(const char* path, const GLenum type)
{
    std::string sourceString = File::fileToString(path);
    const char* source = sourceString.c_str();
    unsigned int id(glCreateShader(type));
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);
    int result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == false)
    {
        std::string str_type;
        switch (type)
        {
        case GL_VERTEX_SHADER:
            str_type = "VERTEX";
            break;
        case GL_FRAGMENT_SHADER:
            str_type = "FRAGMENT";
            break;
        case GL_COMPUTE_SHADER:
            str_type = "COMPUTE";
            break;
        default:
            break;
        }
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(sizeof(char) * length);
        glGetShaderInfoLog(id, length, &length, message);
        Log::error(message);
        glDeleteShader(id);
    }
    glAttachShader(this->id, id);
}

void Shader::compile()
{
    glLinkProgram(id);
    int result = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    if (result == false)
    {
        int length{};
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(sizeof(char) * length);
        glGetProgramInfoLog(id, length, nullptr, message);
        Log::error(message);
    }
    glValidateProgram(id);
}

void Shader::bind() const
{
    glUseProgram(id);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform4f(const std::string& name, const float x, const float y, const float z, const float w) const
{
    glProgramUniform4f(id, getUniformLocation(name), x, y, z, w);
}

void Shader::setUniform3f(const std::string& name, const float x, const float y, const float z) const
{
    glProgramUniform3f(id, getUniformLocation(name), x, y, z);
}

void Shader::setUniform2f(const std::string& name, const float x, const float y) const
{
    glProgramUniform2f(id, getUniformLocation(name), x, y);
}

void Shader::setUniform1f(const std::string& name, const float val) const
{
    glProgramUniform1f(id, getUniformLocation(name), val);
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& val) const
{
    glProgramUniformMatrix4fv(id, getUniformLocation(name), 1, GL_FALSE, &val[0][0]);
}

void Shader::setUniform1i(const std::string& name, const int val) const
{
    glProgramUniform1i(id, getUniformLocation(name), val);
}

void Shader::dispatch(const unsigned int sizeX, const unsigned int sizeY, const unsigned int sizeZ) const
{
    glDispatchCompute(sizeX, sizeY, sizeZ);
}

GLint Shader::getUniformLocation(const std::string& name) const
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
    {
        return uniformLocationCache[name];
    }
    int location = glGetUniformLocation(id, name.c_str());
    if (location == -1)
    {
        Log::warn("Uniform %s not be found. Is it not used?", name.c_str());
    }
    uniformLocationCache[name] = location;
    return location;
}
