#version 410 core

in vec3 normalout;
out vec4 fragColor;

const vec4 green = vec4(0.0f, 1.0f, 0.0f, 1.0f);
const vec4 grey = vec4(0.5f, 0.5f, 0.5f, 1.0f);


void main()
{    
   fragColor = vec4(green);
}