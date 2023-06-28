#version 330 core

in vec3 position;
in vec2 texCoords;

in vec3 particle_position;
in vec2 anim_index;

out vec2 TexCoords;

// texture information
uniform int anim_rows = 1;
uniform int anim_cols = 1;

// camera perspective
uniform mat4 view;
uniform mat4 proj;

void main()
{
	TexCoords = vec2(texCoords.x/anim_cols+anim_index.x/anim_cols,
			texCoords.y/anim_rows+anim_index.y/anim_rows);
	gl_Position = proj*view*vec4(position+particle_position,1.0);
}