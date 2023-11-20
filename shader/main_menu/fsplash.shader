#version 330 core

in vec3 Colour;

out vec4 gbuffer_colour;
out vec4 gbuffer_normals;

uniform float lens_exposure = 1.0;
uniform float gamma = 2.2;

void main()
{
	// colour ouput
	vec3 final = vec3(1.0)-exp(-Colour*lens_exposure);
	final = pow(final,vec3(1.0/gamma));
	gbuffer_colour = vec4(final,1.);

	// pseudo normals (TODO)
	gbuffer_normals = vec4(1.,.0,.0,1.);
}
