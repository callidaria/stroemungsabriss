#version 330 core

out vec4 outColour;

// transition
uniform float ctrans = 1.0f;

void main()
{
	outColour = vec4(.45,0,1,.4*ctrans);
}