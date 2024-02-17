#version 330 core

in vec3 mcolour;

out vec4 outColour;

void main()
{
	outColour = vec4(mcolour,1);
}