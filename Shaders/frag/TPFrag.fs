#version 410 core

layout(location = 0) out vec4 output_colour;

in vec3 fragmentPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightCol;

uniform sampler2D texData;

void main()
{
	//ambient lighting.
	float ambientStrength = 0.4f;	//THIS VALUE HARD CODED IN, CHANGE INTO PROGRAM.
	vec3 ambient = ambientStrength * lightCol;
	
	//normalised light direction.
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragmentPos);
	
	//diffuse lighting.
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightCol;

	//normalised reflected light direction.
	vec3 viewDir = normalize(viewPos - fragmentPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	
	//specular lighting.
	float specularStrength = 0.8f; //THIS VALUE HARD CODED IN, CHANGE INTO PROGRAM.
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64);
	vec3 specular = specularStrength * spec * lightCol;  
	
	//colour of lighting.
	output_colour = vec4((ambient + diffuse + specular), 1.0f) * texture(texData, texCoord);
	
	//BELOW FOR DEBUGGING TO VISUAL NORMAL AND UV DATA
	//output_colour = vec4(normal, 1.0);
	//output_colour = vec4(texCoord, 0.0, 1.0);
}

