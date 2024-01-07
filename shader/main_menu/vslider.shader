#version 330 core

in vec2 position;
in float bmod;

out float mcolour;

// slider value
uniform float sval = .5;
uniform float max_disp = 100;

// displacement
uniform float scroll = .0;

// coordinate system
uniform mat4 view;
uniform mat4 proj;

void main()
{
	mcolour = bmod*sval;
	gl_Position = proj*view*vec4(position.x+max_disp*mcolour,position.y+scroll,0,1);
}
// TODO: wiggly height displacement for a violin-string like effect