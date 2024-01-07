#version 330 core

in float mcolour;

out vec4 outColour;

void main()
{
	outColour = vec4(.4,0,.4*mcolour,1);
}