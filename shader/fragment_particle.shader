#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;

void main()
{
	outColour = vec4(gl_FragCoord.z,0,0,1); //texture(tex,TexCoords);
}