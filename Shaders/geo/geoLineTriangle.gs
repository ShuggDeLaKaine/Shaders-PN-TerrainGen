#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

void build_triangle(vec4 position);


void main()
{
	build_triangle(gl_in[0].gl_Position);
}

void build_triangle(vec4 position)
{
	gl_Position = position + vec4(-2.0f, -2.0f, 0.0f, 0.0f);		//bottom left.
	EmitVertex();
	gl_Position = position + vec4( 2.0f, -2.0f, 0.0f, 0.0f);		//bottom right.
	EmitVertex();
	gl_Position = position + vec4( 0.0f,  2.0f, 0.0f, 0.0f);		//top centre.
	EmitVertex();
	EndPrimitive();
}