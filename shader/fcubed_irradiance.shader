#version 330 core

in vec3 TexCoords;

out vec4 outColour;

// colour source
uniform samplerCube irradiance_map;

// post-processing
uniform float lens_exposure = 1.0;
uniform float gamma = 2.2;

void main()
{
	// extraction
	vec3 colours = texture(irradiance_map,TexCoords).rgb;

	// colour correction
	colours = colours/(colours+vec3(1.0));
	colours = vec3(1.0)-exp(-colours*lens_exposure);
	colours = pow(colours,vec3(1.0/gamma));

	// output
	outColour = vec4(colours,1.0);
}