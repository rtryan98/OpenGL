#version 460 core

layout(location = 0) in vec2 aTexCoord;
layout(location = 0) out vec4 oColor;

uniform sampler2D uTexture;

void main()
{
    oColor = vec4(1.0 - texture(uTexture, aTexCoord).rgb, 1.0);
}
