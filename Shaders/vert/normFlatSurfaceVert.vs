#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
	vec3 normal;
} vs_out;
out vec3 fragPosition;


void main()
{  	
	fragPosition = (model * vec4(aPos, 1.0f)).xyz;
	gl_Position =  projection * view * model * vec4(aPos, 1.0f);
	//normal = aNormal;

	//accomodate scaling and rotation (from view and model) by transforming normals with a normal matrix.
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));

	//geometry shader gets position vectors in world space.
	//so need to transform normal vectors in the same world space.
	//results in a transformed view-space normal vector to pass onto the geometry shader via an interface block (above).
	vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0f)));
}