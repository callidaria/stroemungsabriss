#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform vec3 text_colour = vec3(1,1,1);
uniform sampler2D tex;

void main()
{
	outColour = vec4(text_colour,texture(tex,TexCoords).r);
}
