#version 330 core

in vec3 dcolour;

out vec4 outColour;

void main()
{
	outColour = vec4(dcolour,1.);
}