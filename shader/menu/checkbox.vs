#version 330 core

in vec2 position;
in vec2 drift_mod;

out vec3 dcolour;

// state
uniform int is_active = 0;

// displacement
uniform float scroll = .0;
uniform float aprog = .0;

// camera
uniform mat4 view;
uniform mat4 proj;

void main()
{
	// highlight colour if checked
	dcolour = vec3(.4+.4*aprog,0,0);

	// drift away from center
	vec2 join_check = position+drift_mod*aprog;

	// place into coordinate system
	gl_Position = proj*view*vec4(join_check.x,join_check.y+scroll,.0,1.);
}