#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

out vec2 texCoordsVS;


void main()
{
    texCoordsVS = aTexCoords;  

	//gl_Position = vec4(aPos, 1.0f);
	gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0f);
	//gl_Position = lightSpaceMatrix * vec4(aPos, 1.0f);
}