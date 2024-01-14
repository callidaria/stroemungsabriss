#version 330 core

in vec2 position;
in float dmod;

out vec4 colour;

// displacement
uniform int jmp_dist = 25;
uniform int selection = 0;
uniform int ddsize = 0;

// coordinate system
uniform mat4 proj;
uniform mat4 view;

void main()
{
	// calculate y-axis displacement
	int porigin = int(bool(dmod));
	int pfinal = int(dmod>1.);
	float ydisp = jmp_dist*selection*porigin;
	ydisp += ddsize*jmp_dist*pfinal;

	// translate modification to colour
	colour = vec4(.4,0,.4,.5+.5*porigin);

	// return vertex position
	gl_Position = proj*view*vec4(position.x,position.y-ydisp,.0,1.);
}
