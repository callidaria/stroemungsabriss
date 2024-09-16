#version 330 core

in vec3 Position;
in vec2 TexCoords;
in vec3 Normals;

out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;
out vec4 gbuffer_materials;

uniform sampler2D tex;

void main()
{
	// process texture
	vec4 mix = texture(tex,TexCoords);

	// export colour mix & specular mapping
	gbuffer_colour.rgb = mix.rgb;
	gbuffer_colour.a = 0;

	// export position & shadow map
	gbuffer_position = vec4(Position,0.0);

	// export normals & light emission
	gbuffer_normals.rgb = abs(Normals);
	gbuffer_normals.a = 0;

	// overwrite surface materials
	gbuffer_materials = vec4(0);
}