#version 330 core

out vec4 outColour;

uniform vec3 colour = vec3(.5,0,0);

void main()
{
	outColour = vec4(colour,1.);
}
