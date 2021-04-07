#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

out vec3 eye;
out vec3 worldPos_CS_in;
out vec3 normals;

void main()
{
	normals = normalize((model * vec4(aNormals, 0.0f)).xyz);
	eye = viewPos;
	worldPos_CS_in = (model * vec4(aPos, 1.0f)).xyz;
}