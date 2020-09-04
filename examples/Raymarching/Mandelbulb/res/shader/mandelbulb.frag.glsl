#version 460 core

layout(location = 0) in vec2 aTexCoord;
layout(location = 0) out vec4 oColor;

uniform sampler2D uRaycast;

void main()
{
    oColor = texture(uRaycast, aTexCoord);
}