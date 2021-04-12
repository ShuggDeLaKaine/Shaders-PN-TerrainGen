#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoordsVS;


void main()
{
    texCoordsVS = aTexCoords;  

	//set the position of the vertex, just a pass through.
	gl_Position = vec4(aPos, 1.0);
}