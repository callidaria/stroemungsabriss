#version 330 core

in vec3 Colour;

out vec4 outColour;

uniform float lens_exposure = 1.0;
uniform float gamma = 2.2;

void main()
{
	vec3 final = vec3(1.0)-exp(-Colour*lens_exposure);
	final = pow(final,vec3(1.0/gamma));
	outColour = vec4(final,1.);
}
