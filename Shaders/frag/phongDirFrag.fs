#version 330 core

in vec3 gNormals;
in vec3 gWorldPos_FS_in;

out vec4 fragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};                                                                        

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

//uniform sampler2D texture1;
uniform DirLight dirLight;
uniform Material mat;
uniform vec3 viewPos;


void main()
{   
	//set up vars for view direction, light direcetion, ambient lighting and the norm.
    vec3 viewDir = normalize(viewPos - gWorldPos_FS_in);
	vec3 norm = normalize(gNormals);				//use the input normals from .gs shader here.
	vec3 ambient = dirLight.ambient * mat.ambient;     
    vec3 lightDir = normalize(-dirLight.direction);

    //diffuse lighting shading.
    float diff = max(dot(norm, dirLight.direction), 0.0);

    //specular lighting shading.
    vec3 reflectDir = reflect(-dirLight.direction, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    
	//combine the results.
    vec3 diffuse  = dirLight.diffuse  * (diff * mat.diffuse);
    vec3 specular = dirLight.specular * (spec * mat.specular);
    fragColor = vec4((ambient + diffuse + specular),1.0f);
}