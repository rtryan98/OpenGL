#version 460 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 0) out vec2 oUV;

uniform mat4 uProjection;

void main()
{
    oUV = aUV;
    gl_Position = uProjection * vec4(aPosition.x, aPosition.y, 0.0, 1.0);
}
