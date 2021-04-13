#version 330 core

in vec2 texCoordsVS;

uniform sampler2D sceneTexture;

out vec4 fragColor;


void main()
{
	//COLOUR STUFF.
	vec4 original = texture2D(sceneTexture, texCoordsVS);
	fragColor = original;

	//to get the negative/inverse of the image.
	//fragColor = 1 - original;

	//greyscale effect
	//float average = (original.x + original.y + original.z) / 3.0f;
	//fragColor = vec4(vec3(average), 1.0f);
}
