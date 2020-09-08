#version 460 core

layout(location = 0) in vec2 aPostion;

void main()
{
    gl_Position = vec4(aPostion, 0.0, 1.0);
}
