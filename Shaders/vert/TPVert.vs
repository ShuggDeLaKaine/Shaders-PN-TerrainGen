#version 330 core

layout(location = 0) in vec3 input_vertexPosition;
layout(location = 1) in vec3 input_vertexNormal;
layout(location = 2) in vec2 input_texCoord;

out vec3 fragmentPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragmentPos = vec3(model * vec4(input_vertexPosition, 1.0f));
	
	normal      = mat3(transpose(inverse(model))) * input_vertexNormal;
	
	texCoord    = vec2(input_texCoord.x, input_texCoord.y);
	
	gl_Position = projection * view * model * vec4(input_vertexPosition, 1.0f);
}
