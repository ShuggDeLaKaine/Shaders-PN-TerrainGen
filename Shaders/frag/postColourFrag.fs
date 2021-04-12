#version 330 core

in vec2 texCoordsVS;

uniform sampler2D sceneTexture;

out vec4 fragColor;


void main()
{
	//COLOUR STUFF.
	fragColor = texture(sceneTexture, texCoordsVS);
}
