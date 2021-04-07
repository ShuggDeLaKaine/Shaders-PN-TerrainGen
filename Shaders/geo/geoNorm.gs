#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 projection;

in VS_OUT {
	vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.2f;

void main() 
{  
	for(int i = 0; i < 3; i++)
	{
		gl_Position = projection *  gl_in[i].gl_Position;
		EmitVertex();
		gl_Position = projection *  (gl_in[i].gl_Position + vec4(gs_in[i].normal, 0.0f) * MAGNITUDE);
		EmitVertex();
		EndPrimitive();
	}
}