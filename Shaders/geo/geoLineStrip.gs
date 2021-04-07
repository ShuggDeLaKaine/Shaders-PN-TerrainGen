#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

void build_strip(vec4 position);


void main()
{
	build_strip(gl_in[0].gl_Position);
}

void build_strip(vec4 position)
{
	gl_Position = position + vec4(-5.0f, -2.5f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = position + vec4( 5.0f,  2.5f, 0.0f, 0.0f);
	EmitVertex();
	EndPrimitive();
}