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
	outColour = vec4(texture(irradiance_map,TexCoords).rgb,1.0);
}