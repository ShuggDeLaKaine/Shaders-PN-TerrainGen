#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPosition;

out vec2 uvCoords;


void main() 
{  
	//create view/projection matrices.
	mat4 VP = projection * view;
	
	//set vars position, toCamera, Up and Right.
	vec3 position = gl_in[0].gl_Position.xyz;
	vec3 toCamera = normalize(cameraPosition - position);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = cross(toCamera, up);


	//make a quad from the single vertex to take the texture.
	//bottom-left vertex of quad
	gl_Position = VP * vec4(position, 1.0f);
	uvCoords = vec2(0.0f, 0.0f);		
	EmitVertex();

	//top-left of quad.
	position.y += 1.0f;
	gl_Position = VP * vec4(position, 1.0f);
	uvCoords = vec2(0.0f, 1.0f);		
	EmitVertex();

	//bottom-right of quad.
	position.y -= 1.0f;
	position += right;
	gl_Position = VP * vec4(position, 1.0f);
	uvCoords = vec2(1.0f, 0.0f);
	EmitVertex();

	//top-right of quad.
	position.y += 1.0f;
	gl_Position = VP * vec4(position, 1.0f);
	uvCoords = vec2(1.0f, 1.0f);
	EmitVertex();

	//end and send primitive
    EndPrimitive();
}