#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;		//passing this to calculate shadows in .frag

out vec3 posVS;
out vec2 texCoordsVS;
out vec4 fragPosLightSpaceVS;


void main()
{
	texCoordsVS = aTexCoords;
	posVS = (model * vec4(aPos, 1.0f)).xyz;
	fragPosLightSpaceVS = lightSpaceMatrix * vec4(posVS, 1.0f);		//point as the light sees it.
}