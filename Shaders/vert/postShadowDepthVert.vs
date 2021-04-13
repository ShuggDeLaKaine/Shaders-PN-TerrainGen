#version 330 core

layout (location = 0) in vec3 aPos;

//pass matrix to transform point to light space, looking at scene from lights view
uniform mat4 lightSpaceMatrix;  
uniform mat4 model;


void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0f);
}