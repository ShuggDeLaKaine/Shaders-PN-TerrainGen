#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
	vec3 colour;
} gs_in[];
out vec3 fragColourGS;

void build_house(vec4 position);


void main()
{
	build_house(gl_in[0].gl_Position);
}

void build_house(vec4 position)
{
	fragColourGS = gs_in[0].colour;		//using gs_in[0] as just the one input vertex.

	gl_Position = position + vec4(-2.0f, -2.0f, 0.0f, 0.0f);		//bottom left.
	EmitVertex();
	gl_Position = position + vec4( 2.0f, -2.0f, 0.0f, 0.0f);		//bottom right.
	EmitVertex();
	gl_Position = position + vec4(-2.0f,  2.0f, 0.0f, 0.0f);		//top left.
	EmitVertex();
	gl_Position = position + vec4( 2.0f,  2.0f, 0.0f, 0.0f);		//top left.
	EmitVertex();
	gl_Position = position + vec4( 0.0f,  4.0f, 0.0f, 0.0f);		//top triangle.
	fragColourGS = vec3(1.0f, 1.0f, 1.0f);
	EmitVertex();
	EndPrimitive();	
}