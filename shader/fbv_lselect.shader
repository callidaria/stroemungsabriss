#version 330 core

in vec2 position;
in vec2 index;

uniform vec2 idx_mod[4];

uniform mat4 proj;
uniform mat4 view;

void main()
{
	gl_Position = proj*view*vec4(position+idx_mod[int(index)],0,1);
}