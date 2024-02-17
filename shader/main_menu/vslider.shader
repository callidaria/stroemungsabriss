#version 330 core

in vec2 position;
in float bmod;

out vec3 mcolour;

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
	// positioning
	float edge_influence = bmod*sval;
	gl_Position = proj*view*vec4(position.x+max_disp*edge_influence,position.y+scroll,0,1);

	// tequila sunset effect
	float inv_influence = 1.-edge_influence;
	mcolour = vec3(.4+.6*edge_influence,.4*edge_influence,.4*inv_influence);
}
// TODO: wiggly height displacement for a violin-string like effect