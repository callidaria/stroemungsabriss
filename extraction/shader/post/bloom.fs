#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;
uniform sampler2D bloom;

void main(void)
{
	vec4 c_tex = texture(tex,TexCoords);
	vec4 c_bloom = texture(bloom,TexCoords);
	outColour = c_tex*0.7+c_bloom*1.5;
}
