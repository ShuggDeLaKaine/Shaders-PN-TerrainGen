#version 410 core

uniform sampler2D billboard;

in vec2 uvCoords;
out vec4 fragColour;


float threshold = 0.35f;
const vec4 green = vec4(0.0f, 1.0f, 0.0f, 1.0f);


void main()
{    
	//fragColour = green;
	fragColour = texture(billboard, -uvCoords);
	
	if(fragColour.r > threshold &&
	   fragColour.g > threshold &&
	   fragColour.b > threshold)
	{
		discard;
	}
}