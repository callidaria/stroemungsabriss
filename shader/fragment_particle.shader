#version 330 core

in vec2 TexCoords;
in float dproj;

out vec4 outColour;

uniform sampler2D tex;

uniform mat4 proj;

void main()
{
	outColour = texture(tex,TexCoords);
}