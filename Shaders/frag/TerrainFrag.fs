#version 410 core

in vec3 posGS;
in vec3 normalsGS;
in float visibilityGS;
//in vec2 tessTexTES;

out vec4 finalFragColour;

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

float calculateShadows(vec4 fragPLS);

uniform DirLight dirLight;
uniform Material mat;

uniform vec3 viewPos;
uniform vec3 sky;
uniform int scale;

uniform bool useFog;
uniform bool useJustTextures;
uniform bool useTexturesAndColour;
uniform bool useShadowMapping;

uniform sampler2D grassTexture;
uniform sampler2D rockTexture;
uniform sampler2D dirtTexture;
uniform sampler2D soilTexture;
uniform sampler2D snowTexture;

uniform mat4 lightSpaceMatrix;		//passing this to calculate shadows in .frag
uniform sampler2D shadowMap;

//for debugging the fog
in float inNegative;
in float overZero;
in float overOne;
in float noChanges;

void main()
{    
	//check whether these two bool options are true, if so, set useJustTextures to false and just use useTexturesAndColour.
	//if(useJustTextures == true && useTexturesAndColour == true)
	//	useJustTextures = false;

	bool inDebug = false;

	if(inDebug)
	{
		if(visibilityGS < 0.0f)
			finalFragColour = vec4(1.0f, 0.0f, 0.0f, 1.0f);		//RED
		else if(visibilityGS == 0.0f)
			finalFragColour = vec4(0.0f, 1.0f, 0.0f, 1.0f);		//GREEN			//THIS IS FLAGGING - HAS A VALUE OF 0.0F....
		else if(visibilityGS > 0.0f)
			finalFragColour = vec4(0.0f, 0.0f, 1.0f, 1.0f);		//BLUE
		else
			finalFragColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);		//WHITE
	}
	else
	{
		//BLINN-PHONG
		vec4 lightingColour = vec4(0.0f);
		vec3 norm = normalsGS;	

		//set up vars for view & light direcetions.
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
		lightingColour = vec4((ambient + diffuse + specular), 1.0f);

		//terrain colour & texture attempt...
		vec4 terrainColour = vec4(0.0f);
		float height = posGS.y / scale;

	vec4 vegGreen = vec4(0.22f, 0.31f, 0.18f, 0.0f);
		vec4 dirtBrown = vec4(0.56f, 0.47f, 0.26f, 0.0f);
		vec4 darkBrown = vec4(0.26f, 0.13f, 0.0f, 0.0f);
		vec4 stoneGrey = vec4(0.21f, 0.21f, 0.21f, 0.0f);
		vec4 darkGrey = vec4(0.22f, 0.24f, 0.2f, 0.0f);
		vec4 snowWhite = vec4(0.9f, 0.92f, 0.92f, 0.0f);

		//if want to just use textures, otherwise just use colours.
		if(useJustTextures) {
			if(height <= 0.1f)
				terrainColour = vec4(vec3(texture(soilTexture, posGS.xz).rgb), 1.0f);
			else if(height > 0.1f && height <= 0.2f)
				terrainColour = vec4(vec3(mix(vec3(texture(soilTexture, posGS.xz)), vec3(texture(dirtTexture, posGS.xz)), smoothstep(0.07f, 0.23f, height)).rgb), 1.0f);
			else if (height > 0.2f && height <= 0.4f)
				terrainColour = vec4(vec3(mix(vec3(texture(dirtTexture, posGS.xz)), vec3(texture(grassTexture, posGS.xz)), smoothstep(0.17f, 0.43f, height)).rgb), 1.0f);
			else if(height > 0.4f && height <= 0.5f )
				terrainColour = vec4(vec3(mix(vec3(texture(grassTexture, posGS.xz)), vec3(texture(rockTexture, posGS.xz)), smoothstep(0.37f, 0.53f, height)).rgb), 1.0f);
			else if(height > 0.5f && height <= 0.8f)
				terrainColour = vec4(vec3(mix(vec3(texture(rockTexture, posGS.xz)), vec3(texture(snowTexture, posGS.xz)), smoothstep(0.47f, 0.83f, height)).rgb), 1.0f);
			else if(height > 0.8f)
				terrainColour = vec4(vec3(texture(snowTexture, posGS.xz).rgb), 1.0f);
		} else {
			if(height <= 0.1f)
				terrainColour = vec4(darkBrown.rgb, 1.0f);
			else if(height > 0.1f && height <= 0.2f)
				terrainColour = vec4(vec3(mix(darkBrown, dirtBrown, smoothstep(0.08f, 0.22f, height)).rgb), 1.0f);
			else if(height > 0.2f && height <= 0.4f)
				terrainColour = vec4(vec3(mix(dirtBrown, vegGreen, smoothstep(0.18f, 0.42f, height)).rgb), 1.0f);
			else if(height > 0.4f && height <= 0.5f)
				terrainColour = vec4(vec3(mix(vegGreen, darkGrey, smoothstep(0.38f, 0.52f, height)).rgb), 1.0f);
			else if(height > 0.5f && height <= 0.8f)
				terrainColour = vec4(vec3(mix(darkGrey, snowWhite, smoothstep(0.48f, 0.82f, height)).rgb), 1.0f);
			else if(height > 0.8f)
				terrainColour = vec4(snowWhite.rgb, 1.0f);
		}

		//if you want to use textures AND colours together.
		if(useTexturesAndColour)
		{
			if(height <= 0.1f) {
				terrainColour = vec4(darkBrown.rgb, 1.0f);
				terrainColour = terrainColour + vec4(vec3(texture(soilTexture, posGS.xz).rgb), 1.0f);
			} else if(height > 0.1f && height <= 0.2f) {
				terrainColour = vec4(vec3(mix(darkBrown, dirtBrown, smoothstep(0.08f, 0.22f, height)).rgb), 1.0f);
				terrainColour = terrainColour + vec4(vec3(mix(vec3(texture(soilTexture, posGS.xz)), vec3(texture(dirtTexture, posGS.xz)), smoothstep(0.07f, 0.23f, height)).rgb), 1.0f);
			} else if(height > 0.2f && height <= 0.4f) {
				terrainColour = vec4(vec3(mix(dirtBrown, vegGreen, smoothstep(0.18f, 0.42f, height)).rgb), 1.0f);
				terrainColour = terrainColour + vec4(vec3(mix(vec3(texture(dirtTexture, posGS.xz)), vec3(texture(grassTexture, posGS.xz)), smoothstep(0.17f, 0.43f, height)).rgb), 1.0f);
			} else if(height > 0.4f && height <= 0.5f) {
				terrainColour = vec4(vec3(mix(vegGreen, darkGrey, smoothstep(0.38f, 0.52f, height)).rgb), 1.0f);
				terrainColour = terrainColour + vec4(vec3(mix(vec3(texture(grassTexture, posGS.xz)), vec3(texture(rockTexture, posGS.xz)), smoothstep(0.37f, 0.53f, height)).rgb), 1.0f);
			} else if(height > 0.5f && height <= 0.8f) {
				terrainColour = vec4(vec3(mix(darkGrey, snowWhite, smoothstep(0.48f, 0.82f, height)).rgb), 1.0f);
				terrainColour = terrainColour + vec4(vec3(mix(vec3(texture(rockTexture, posGS.xz)), vec3(texture(snowTexture, posGS.xz)), smoothstep(0.47f, 0.83f, height)).rgb), 1.0f);
			} else if(height > 0.8f) {
				terrainColour = vec4(snowWhite.rgb, 1.0f);
				terrainColour = terrainColour+ vec4(vec3(texture(snowTexture, posGS.xz).rgb), 1.0f);
			}
		}

		//combine the height/colour terrain with the lighting colour.
		finalFragColour = terrainColour + lightingColour;

		//if you want to use the fog effect.
		if(useFog)
		{
			//sky.rgb = sky.rgb * 0.5f;
			finalFragColour = mix(vec4(sky, 1.0f), finalFragColour, visibilityGS);
		}

		if(useShadowMapping)
		{
			vec4 fragPosLightSpace = lightSpaceMatrix * vec4(posGS, 1.0f);
			//shadow mapping.
			float shadow = calculateShadows(fragPosLightSpace);
			//combine the shadow result with the exisitng data per frag by working out how much of the fragment is NOT in the shadow.
			finalFragColour = vec4(ambient + (1.0f - shadow) * (diffuse + specular), 1.0f);
			//finalFragColour = vec3(vec3(shadow), 1.0f);
		}
	}
}

float calculateShadows(vec4 fragPLS)
{
	float shadow = 0.0f;
	float offset = 0.015f;	//offset to deal with the 'shadow acne' problem.

	//divide values in range [-1, 1].
	vec3 projectionCoords = fragPLS.xyz / fragPLS.w;

	//transform to [0, 1] range.
	projectionCoords = projectionCoords * 0.5f + 0.5f;

	//sample from shadow map (returns a float, called closestDepth).
	float closestDepth = texture(shadowMap, projectionCoords.xy).r;

	//get depth of current fragment from the lights perspective (called currentDepth).
	float currentDepth = projectionCoords.z;

	//shadow anti-aliasing (deals with 'jaggie' problem)
	vec2 texelSize = 1.0f / textureSize(shadowMap, 0);	//returns width & height of texture that's being sampled; divide by 1 to get the size of texel. Used to offset the original xy coordinates in the kernal.
	//loop through the 3 by 3 kernal.
	for(int i = -3; i < 2; i++)
	{
		for(int j = -3; j < 2; j++)
		{
			float PCF = texture(shadowMap, projectionCoords.xy + vec2(i, j) * texelSize).r;
			if(currentDepth - offset > PCF)
				shadow += 1.0f;
		}
	}
	shadow = shadow / 9;	//as we are using a 3 by 3 kernal

	//check whether frag in shadow by checking if currentDepth > closestDepth.
	if(currentDepth - offset > closestDepth)
		shadow = 1.0f;
	if(projectionCoords.z > 1.0f)		//deals with 'outside frustum' problem, outside frustum z value is 1.0f, so full shadow, don't want this.
		shadow = 0.0f;
    return shadow;
}