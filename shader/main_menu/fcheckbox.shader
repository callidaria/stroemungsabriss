#version 330 core

in vec2 colour_mod;

out vec4 outColour;

void main()
{
	outColour = vec4(colour_mod*.7,.7,1);
}