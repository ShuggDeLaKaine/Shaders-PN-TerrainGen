#version 410 core

in vec3 fragPosTES;
in vec2 tessTexTES;

out vec4 fragColor;

void main()
{    
   fragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}