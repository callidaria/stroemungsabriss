#version 330 core

in vec2 TexCoords;

out vec4 outColour;

void main()
{
	outColour = vec4(TexCoords,0,1);
}