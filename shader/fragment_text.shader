#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;
uniform vec4 colour=vec4(1,1,1,1);

void main()
{
	outColour=texture(tex,TexCoords)*colour;
}
