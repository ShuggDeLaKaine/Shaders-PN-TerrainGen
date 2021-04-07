#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 projection;

in VS_OUT {
	vec3 normal;
} gs_in[];
out vec3 normalout;

const float MAGNITUDE = 0.2f;
vec3 GetSurfaceNormal();


void main() 
{  
	//set var with the surfaces normal.
	vec3 surfaceNormal = GetSurfaceNormal();

	//get centre of primitive = (sum of 3 vertices) / 3
	vec4 centrePos = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3;

	//set first vertex on line to the centre position of surface.
	gl_Position = centrePos;
	EmitVertex();

	//create second vertex, starting from centre position of surface, go along surface normal, for as long as magnitude.
	gl_Position = centrePos + vec4(surfaceNormal, 1.0f) * MAGNITUDE;	
	EmitVertex();

	//end primitive.
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