#version 330 core

in vec4 Position;
in vec2 TexCoords;
in mat3 TBN;

out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;
out vec4 gbuffer_materials;

// object material textures
uniform sampler2D tex;
uniform sampler2D sm;
uniform sampler2D emit;
uniform sampler2D nmap;

void main()
{
	// extract colour & specular
	gbuffer_colour = texture(tex,TexCoords);
	gbuffer_colour.a = texture(sm,TexCoords).r;

	// translate position & shadow
	gbuffer_position = vec4(Position.xyz,0.0);

	// translate normals & emission
	vec3 normals = texture(nmap,TexCoords).rgb*2-1;
	gbuffer_normals.rgb = normalize(TBN*normals);
	gbuffer_normals.a = texture(emit,TexCoords).r;

	// overwrite surface materials
	gbuffer_materials = vec4(0);
}
// FIXME: specular & emission map only use one colour channel, merge them?