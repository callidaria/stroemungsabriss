#version 330 core

in vec3 Position;

out vec4 outColour;

uniform sampler2D irradiance_map;

void main()
{
	vec3 norm_position = normalize(Position);
	float phi = atan(norm_position.z,norm_position.x);
	float theta = -asin(norm_position.y);
	vec2 tex_coords = vec2(phi*.1591+.5,theta*.3183+.5);
	outColour = vec4(texture(irradiance_map,tex_coords).rgb,1.0);
}