#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;

void main()
{
	outColour = vec4(texture(tex,TexCoords).r,0,0,1);
    outColour = vec4(1,0,0,1);
}
