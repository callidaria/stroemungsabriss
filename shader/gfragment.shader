#version 330 core

in vec4 Position;
in vec2 TexCoords;
in mat3 TBN;
in vec3 ltp;

out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec3 gbuffer_normals;

uniform sampler2D tex;
uniform sampler2D sm;
uniform sampler2D emit;
uniform sampler2D shadow_map;
uniform sampler2D nmap;

float calculate_shadow();

void main()
{
	// extract colour & specular
	gbuffer_colour = texture(tex,TexCoords);
	gbuffer_colour.a = texture(sm,TexCoords).r;

	// translate position
	gbuffer_position.rgb = Position.rgb;

	// translate normals
	vec3 normals = texture(nmap,TexCoords).rgb*2-1;
	gbuffer_normals = normalize(TBN*normals);

	// translate shadow
	gbuffer_position.a = calculate_shadow();
}

// dynamic shadow map generation
float calculate_shadow()
{
	float curr_depth = ltp.z;
	vec2 texel = 1.0/textureSize(shadow_map,0);
	float out_shadow = 0;
	for (int x=-1;x<=1;x++) {
		for(int y=-1;y<=1;y++) {
			float pcf_depth = texture(shadow_map,ltp.xy+vec2(x,y)*texel).r;
			out_shadow += float(curr_depth-0.0002>pcf_depth);
		}
	} return 1-out_shadow/9.0;
}