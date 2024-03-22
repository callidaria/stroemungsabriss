#version 330 core

in vec2 blurTexCoords[11];

out vec4 outColour;

uniform sampler2D tex;

void main()
{
	outColour = vec4(0.0);
	outColour += texture(tex,blurTexCoords[0]) * 0.0093;
	outColour += texture(tex,blurTexCoords[1]) * 0.028002;
	outColour += texture(tex,blurTexCoords[2]) * 0.065984;
	outColour += texture(tex,blurTexCoords[3]) * 0.121703;
	outColour += texture(tex,blurTexCoords[4]) * 0.175713;
	outColour += texture(tex,blurTexCoords[5]) * 0.198596;
	outColour += texture(tex,blurTexCoords[6]) * 0.175713;
	outColour += texture(tex,blurTexCoords[7]) * 0.121703;
	outColour += texture(tex,blurTexCoords[8]) * 0.065984;
	outColour += texture(tex,blurTexCoords[9]) * 0.028002;
	outColour += texture(tex,blurTexCoords[10]) * 0.0093;
}
