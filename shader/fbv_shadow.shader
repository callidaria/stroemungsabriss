#version 330 core

in vec3 position;

uniform mat4 shadow_mat;
uniform mat4 model=mat4(1.0);

void main()
{
	gl_Position=shadow_mat*model*vec4(position,1.0);
}
