#version 330 core

in vec2 position;
in float bmod;

// displacement
uniform float scroll = .0;

// state
uniform int is_active = 0;

// camera
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj*view*vec4(position,0,1);
}