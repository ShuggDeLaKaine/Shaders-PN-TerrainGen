#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 fragmentColour[];
out vec3 fragmentColourGS;


void main() 
{  
	for(int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		fragmentColourGS = fragmentColour[i];
		EmitVertex();
	}
	
    EndPrimitive();
}