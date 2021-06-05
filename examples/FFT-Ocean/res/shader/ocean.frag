#version 460 core

layout(location = 0) out vec4 oColor;

layout(location = 0) in vec2 iUV;

void main()
{
    oColor = vec4(iUV, 0.0, 1.0);
}
