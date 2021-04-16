#version 330 core

in vec2 texCoordsVS;

void createFX();
void nightVisionFX();

uniform sampler2D sceneTexture;

uniform bool useInverse;
uniform bool useGreyscale;
uniform bool useIntoxicateFX;
uniform bool useBlurFX;
uniform bool useOutlineFX;
uniform bool useNightVisionFX;

out vec4 fragColor;


void main()
{
	vec4 original = texture2D(sceneTexture, texCoordsVS);

	if(useInverse)
		//to get the negative/inverse of the image.
		fragColor = 1.0f - original;
	else if(useGreyscale) 
	{
		//greyscale effect
		float average = (original.x + original.y + original.z) / 3.0f;
		fragColor = vec4(vec3(average), 1.0f);
	} 
	else if(useIntoxicateFX)
		createFX();
	else if(useBlurFX)
		createFX();
	else if(useOutlineFX)
		createFX();
	else if(useNightVisionFX)
		nightVisionFX();
	else
		fragColor = original;
}


void createFX()
{
	//create offsets.
	const float offset = 1.0f / 200.0f;
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

	float kernel[9];

	//create your kernel
	if(useIntoxicateFX)
	{
		kernel = float[](
		    -1, -1, -1,
		    -1,  9, -1,
		    -1, -1, -1
		);
	} else if (useBlurFX) {
		//values all add up to 16, so need to be divided by 16 to prevent extremely BRIGHT colours.
		kernel = float[](
			1.0f / 16.0f,	2.0f / 16.0f,	1.0f / 16.0f,
			2.0f / 16.0f,	4.0f / 16.0f,	2.0f / 16.0f,
			1.0f / 16.0f,	2.0f / 16.0f,	1.0f / 16.0f  
		);
	} else if (useOutlineFX) {
		//make sure they all add up to 1.0f.
		kernel = float[](
			1.0f,	1.0f,	1.0f,
			1.0f,  -8.0f,	1.0f,
			1.0f,	1.0f,	1.0f  
		);
	}

	//create texture sampling array and loop through, setting the text coords to the 2d image, plus the offsets.
	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(sceneTexture, texCoordsVS.st + offsets[i]));
    }

	//loop through colours and apply to frag colours.
    vec3 colour = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < 9; i++)
	{
        colour += sampleTex[i] * kernel[i];
    }

    fragColor = vec4(colour, 1.0f);
}

void nightVisionFX()
{
	float intensity = 1.2f;
	float contrast = 0.8f;  
	
	//sample scene texture.
	vec3 sceneCol = texture(sceneTexture, texCoordsVS.st).rgb;   
	
	//colour intensity, night vision so green.
	const vec3 lumon = vec3(0.28f, 0.63f, 0.09f);
	float inten = dot(lumon, sceneCol) ;

	//contrast [0-1].
	inten = clamp(contrast * (inten - 0.5f) + 0.5f, 0.0f, 1.0f);

	//final green [0-1].
	float finalGreen = clamp(inten / 0.55f, 0.0f, 1.0f) * intensity;

	//maxing green for vision colour.
	vec3 visionColor = vec3(0.0f, finalGreen, 0.0f); 

	//final frag colour.
	fragColor = vec4((sceneCol * visionColor), 1.0f);
}