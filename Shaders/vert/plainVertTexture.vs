#version 330 core

layout (location = 0) in vec3 a_vertexPosition;
layout (location = 1) in vec3 a_vertexColour;
//layout (location = 2) in vec2 a_vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragmentColour;
//out vec2 texUV;

void main()
{
	fragmentColour = vec3(1.0f); //a_vertexColour;
	//textUV = a_vertexUV;
    gl_Position = projection * view * model * vec4(a_vertexPosition, 1.0f);
}