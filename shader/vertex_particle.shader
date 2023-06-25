#version 330 core

in vec3 position;
in vec2 texCoords;

in vec3 particle_position;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 proj;

void main()
{
	TexCoords = texCoords;
	gl_Position = proj*view*vec4(position,1.0);
}