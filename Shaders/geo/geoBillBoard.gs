#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 4) out;

uniform mat4 view;
uniform mat4 projection;

in vec3 fragmentColour[];
out vec3 fragmentColourGS;

out vec2 uv;


void main() {  
	//VP = vertex position
	mat4 VP = projection * view;
	
	//get position of vertex and store in vec3 pos.
	vec3 pos = gl_in[0].gl_Position.xyz;
	fragmentColourGS = fragmentColour[0];
	EmitVertex();

	//establish vars for what is Up, Right and towards the camera.
	vec3 toCamera = normalize(cameraPos - pos);
	vec3 up = vec3(0.0f, 1.0f, 0.0);
	vec3 right = cross(toCamera, up);


    gl_Position = VP * vec4(pos, 1.0f);
    uv = vec2(0.0f, 0.0f);
    EmitVertex();

    pos.y += 1.0;
    gl_Position = VP * vec4(pos, 1.0f);
    uv = vec2(0.0f, 1.0f);
    EmitVertex();

    pos.y -= 1.0f;
    pos += right;
    gl_Position = VP * vec4(pos, 1.0f);
    uv = vec2(1.0f, 0.0f);
    EmitVertex();

    pos.y += 1.0f;
    gl_Position = VP * vec4(pos, 1.0f);
    uv = vec2(1.0f, 1.0f);
    EmitVertex();
	
    EndPrimitive();
}