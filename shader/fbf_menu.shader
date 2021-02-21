#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;
uniform sampler2D splash;

void main()
{
	vec4 proc = texture(tex,TexCoords);
	vec4 sproc = texture(splash,TexCoords);
	outColour = vec4(proc.rgb+sproc.rgb,1.0);
}
