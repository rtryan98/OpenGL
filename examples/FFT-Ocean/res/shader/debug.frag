#version 460 core

layout(location = 0) out vec4 oColor;

layout(location = 0) in vec2 iUV;

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
    oColor = vec4(texture(uTexture, iUV).rg, 0.0, 1.0);
}
