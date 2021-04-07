#version 410 core

in vec3 fragmentColourGS;
//in vec2 texUV;
out vec4 fragColor;

void main()
{    
	fragColor = vec4(fragmentColourGS, 1.0f);
}