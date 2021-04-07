#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

vec3 getNormal();

uniform bool useLowPolyFlatSurfaces;

in vec3 posTES[];
in vec3 normalsTES[];
in float visibilityTES[];

out vec3 posGS;
out vec3 normalsGS;
out float visibilityGS;

void main()
{
	for(int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		posGS = posTES[i];

		if(useLowPolyFlatSurfaces)
			normalsGS = getNormal();	//for flat norm surfaces (aka that low-poly look).
		else
			normalsGS = normalsTES[i];

		visibilityGS = visibilityTES[i];
		EmitVertex();
	}
	EndPrimitive();
}

//function to return the normal using the gl_positions.
vec3 getNormal()
{
	vec3 a = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
	return normalize(cross(a, b));
}