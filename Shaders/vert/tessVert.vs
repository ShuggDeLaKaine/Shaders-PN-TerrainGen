#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;

uniform mat4 model;

out vec3 posVS; 
out vec3 normVS; 


void main()
{
	normVS = aNormals; 
	posVS = (model * vec4(aPos,1.0)).xyz; 
}