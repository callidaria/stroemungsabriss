#version 330 core

in vec3 Position;

out vec4 outColour;

uniform sampler2D irradiance_map;

void main()
{
	float phi = atan(Position.z,Position.x);
	float theta = -asin(Position.y);
	vec2 tex_coords = vec2(phi*.1591+.5,theta*.3183+.5);
	outColour = vec4(texture(irradiance_map,tex_coords).rgb,1.0);
}