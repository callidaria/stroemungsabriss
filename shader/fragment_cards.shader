#version 330 core

in vec3 Position;
in vec2 TexCoords;
in vec3 Normals;
in vec4 shadow_origin;

out vec4 outColour;

uniform sampler2D tex;
uniform sampler2D shadow_map;

struct light_sun {
	vec3 pos;
	vec3 col;
	float ins;
};
uniform light_sun al[1];

float calc_shadow(vec4 tlp);

void main()
{
	// process texture
	vec4 mix = texture(tex,TexCoords);

	// simulate lighting
	vec3 light_dir = normalize(al[0].pos-Position);
	float diffusion = max(abs(dot(Normals,light_dir)),.1);
	vec3 mix_lmult = diffusion*al[0].col;

	// receive shadows
	float shadow = calc_shadow(shadow_origin);
	mix_lmult *= (1-shadow);

	// return colour mix
	outColour = vec4(mix_lmult*mix.rgb,mix.a);
}

float calc_shadow(vec4 ltp)
{
	vec3 pc = ltp.xyz/ltp.w;
	pc = pc*.5+.5;
	float curr_depth = pc.z;
	float out_shadow = 0;
	vec2 texel = 1.0/textureSize(shadow_map,0);
	for (int x=-1;x<=1;x++) {
		for (int y=-1;y<=1;y++) {
			float pcf_depth = texture(shadow_map,pc.xy+vec2(x,y)*texel).r;
			out_shadow += float(curr_depth-0.00001>pcf_depth);
		}
	} out_shadow /= 9.0;
	return out_shadow;
}