#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform mat4 projection;

in VS_OUT {
	vec3 normal;
} gs_in[];
in vec3 fragPosition[];

out vec3 normalGSout;
out vec3 worldPosition;

vec3 GetSurfaceNormal();

void main() 
{  
	//get the surface normal.
	vec3 surfaceNormal = GetSurfaceNormal();

	//loop through vertices and set each ones normal to the surface normal.
	for(int i = 0; i < 3; i++)
	{
		gl_Position = gl_in[i].gl_Position;
		worldPosition = fragPosition[i];
		normalGSout = surfaceNormal;
		EmitVertex();
	}

	//create the primitve to be sent off to frag.
	EndPrimitive();
}

vec3 GetSurfaceNormal()
{
	//var a = V0 - V1
	//var b = V2 - V1
	//Normal = normalise(cross(A, B));

	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}