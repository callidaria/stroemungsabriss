#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;
uniform float expo = 1.0;

void main()
{
	vec3 mix = texture(tex,TexCoords).rgb;
	vec3 out_res = vec3(1.0)-exp(-mix*expo);
	outColour = vec4(out_res,1.0);
}
