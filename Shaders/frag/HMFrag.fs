#version 410 core

in vec3 posGS;
in vec3 normalsGS;
//in vec2 tessTexTES;

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

uniform DirLight dirLight;
uniform Material mat;
uniform vec3 viewPos;


void main()
{    
	//PHONG
    //set up vars for view direction, light direcetion, ambient lighting and the norm.
	//vec3 norm = normalsGS;		
    //vec3 viewDir = normalize(viewPos - posGS);				
    //vec3 ambient = dirLight.ambient * mat.ambient;     
    //vec3 lightDir = normalize(-dirLight.direction);

    //diffuse lighting shading.
    //float diff = max(dot(norm, dirLight.direction), 0.0f);

    //specular lighting shading.
    //vec3 reflectDir = reflect(-dirLight.direction, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0f), mat.shininess);
    
	//combine the results.
    //vec3 diffuse  = dirLight.diffuse  * (diff * mat.diffuse);
    //vec3 specular = dirLight.specular * (spec * mat.specular);
    //fragColor = vec4((ambient + diffuse + specular), 1.0f);



	//BLINN-PHONG
	vec3 norm = normalsGS;	

    //set up vars for view direction, light direcetion, ambient lighting and the norm.
    vec3 lightDir = normalize(dirLight.direction - posGS);
	vec3 viewDir = normalize(viewPos - posGS);		

	//ambient lighting shading.
	vec3 ambient = dirLight.ambient * mat.ambient;    

    //diffuse lighting shading.
    float diff = max(dot(norm, dirLight.direction), 0.0f);

    //specular lighting shading.
	vec3 halfwayLight = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayLight), 0.0f), mat.shininess);
    
	//combine the results.
    vec3 diffuse  = dirLight.diffuse  * (diff * mat.diffuse);
    vec3 specular = dirLight.specular * (spec * mat.specular);
    fragColor = vec4((ambient + diffuse + specular), 1.0f);
}