#version 330 core

in vec3 coldef;

out vec4 outColour;

void main()
{
	outColour = vec4(coldef.xyz,.4f);
}
