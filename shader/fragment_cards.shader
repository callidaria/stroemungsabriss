#version 330 core

in vec3 Position;
in vec2 TexCoords;
in vec3 Normals;

out vec4 outColour;

uniform sampler2D tex;

struct light_sun {
	vec3 pos;
	vec3 col;
	float ins;
};
uniform light_sun al[1];

void main()
{
	// process texture
	vec4 mix = texture(tex,TexCoords);

	// simulate lighting
	vec3 light_dir = normalize(al[0].pos-Position);
	float diffusion = max(abs(dot(Normals,light_dir)),.1);
	vec3 mix_lmult = diffusion*al[0].col;

	// return colour mix
	outColour = vec4(mix_lmult*mix.rgb,mix.a);
}