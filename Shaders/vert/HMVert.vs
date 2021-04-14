#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;

out vec3 posVS;
out vec2 texCoordsVS;


void main()
{
	texCoordsVS = aTexCoords;
	posVS = (model * vec4(aPos, 1.0f)).xyz;
}