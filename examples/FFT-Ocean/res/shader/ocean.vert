#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

layout(location = 0) out vec2 oUV;

uniform mat4 uProjection;
uniform mat4 uView;
uniform float uTime;

void main()
{
    oUV = aUV;
    gl_Position = uProjection * uView * vec4(aPosition.x, aPosition.y + sin(uTime + aPosition.z), aPosition.z + cos(uTime + aPosition.x) , 1.0);
}
