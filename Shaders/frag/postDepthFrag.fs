#version 330 core

in vec2 texCoordsVS;

uniform sampler2D depthMap;

float lineariseTheDepth(float depth);
const float nearPlane = 0.1f;
const float farPlane = 1000.0f;

out vec4 fragColor;


void main()
{
	//DEPTH STUFF 
	float depth = texture(depthMap, texCoordsVS).r;

	//for perspective view.
	fragColor = vec4(vec3(lineariseTheDepth(depth) / farPlane), 1.0f); 

	//for orthographic view.
	//fragColor = vec4(vec3(depth), 1.0f);
}

float lineariseTheDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}