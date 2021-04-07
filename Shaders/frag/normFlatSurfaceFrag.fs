#version 410 core

in vec3 normalGSout;
in vec3 worldPosition;
out vec4 fragColour;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shine;
};

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirectionalLight dirLight;
uniform Material mat;
uniform vec3 viewPosition;

const vec4 green = vec4(0.0f, 1.0f, 0.0f, 1.0f);
const vec4 grey = vec4(0.5f, 0.5f, 0.5f, 1.0f);


void main()
{    
   //fragColour = vec4(grey);

   //set up vars for view direction, light direcetion, ambient lighting and the norm.
   vec3 viewDir = normalize(viewPosition - worldPosition);
   vec3 norm = normalize(normalGSout);			//use the input normals from .gs shader here.
   vec3 ambient = dirLight.ambient * mat.ambient;
   vec3 lightDir = normalize(-dirLight.direction);

   //diffuse lighting shading.
   float diff = max(dot(norm, dirLight.direction), 0.0f);

   //specular lighting shading.
   vec3 reflectDir = reflect(-dirLight.direction, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0f), mat.shine);

   //combine the results.
   vec3 diffuse = dirLight.diffuse * (diff * mat.diffuse);
   vec3 specular = dirLight.specular * (spec * mat.specular);
   fragColour = vec4((ambient + diffuse + specular), 1.0f);
}