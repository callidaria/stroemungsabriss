#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;
uniform sampler2D overlay;

void main()
{
	// textures
	vec4 splash_tex = texture(tex,TexCoords);		// base buffer
	vec4 overlay_tex = texture(overlay,TexCoords);	// title overlay

	// overwrite splash with native colour
	float ovwr = (.5-overlay_tex.b)*2;							// check for title
	float intr = (1-ovwr)*int(splash_tex.r+splash_tex.g>.25);	// check title intersections

	// colour inversion for selection splash
	int inv = int(splash_tex.g>.25);
	overlay_tex = (1-inv)*overlay_tex+inv*vec4(0,0,0,int(overlay_tex.g>.25));

	// output pixel colour
	outColour = splash_tex*ovwr;
	outColour += overlay_tex*intr;  // §§nice effect if you replace *intr with *ovwr
}