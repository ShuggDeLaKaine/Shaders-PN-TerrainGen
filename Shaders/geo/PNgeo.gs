#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

vec3 getNormal();

uniform float normLength;

in vec3 worldPos_FS_in[];
in vec3 normals[];

out vec3 gNormals;
out vec3 gWorldPos_FS_in;
out vec3 gFragPos;


void main()
{
	for(int i = 0; i < 3; i++)
	{
		gFragPos = vec3(0.0f);
		gl_Position = gl_in[i].gl_Position;
		gWorldPos_FS_in = worldPos_FS_in[i];
		gNormals = getNormal();
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