#version 330 core

in vec3 Colour;

out vec4 outColour;

void main()
{
	outColour = vec4(Colour,1);
}
