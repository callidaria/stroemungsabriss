#version 330 core

in vec2 position;
in vec2 bmod;
in vec2 drift_mod;

out vec2 colour_mod;

// state
uniform int is_active = 0;

// displacement
uniform float scroll = .0;
uniform float qsize = 10.;
uniform float aprog = .0;

// camera
uniform mat4 view;
uniform mat4 proj;

void main()
{
	// checking animation
	colour_mod = bmod;
	vec2 join_check = position+bmod*qsize*aprog;

	// drift away from center
	join_check += drift_mod*2.*aprog;

	// place into coordinate system
	gl_Position = proj*view*vec4(join_check.x,join_check.y+scroll,.0,1.);
}