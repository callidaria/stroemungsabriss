#version 330 core

in vec2 position;
in vec2 bmod;

out vec2 colour_mod;

// state
uniform int is_active = 0;

// displacement
uniform float scroll = .0;
uniform float qsize = 10;
uniform float aprog = 1;

// camera
uniform mat4 view;
uniform mat4 proj;

void main()
{
	colour_mod = bmod;
	vec2 join_check = position+bmod*qsize*aprog;
	gl_Position = proj*view*vec4(join_check.x,join_check.y+scroll,0,1);
}