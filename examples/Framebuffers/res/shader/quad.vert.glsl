#version 460 core

layout(location = 0) in vec2 aPostion;
layout(location = 1) in vec2 aTexCoord;
layout(location = 0) out vec2 oTexCoord;

void main()
{
    oTexCoord = aTexCoord;
    gl_Position = vec4(aPostion, 0.0, 1.0);
}
