#version 330 core

in vec2 position;
in float edge_id;

in float ofs;
in float wdt;
in float dmg;
in float edg_trans[4];

// camera
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	// temporary changable position
	vec2 pos = position;
	int edg_id = int(edge_id);

	// situational variables setup
	float wfac = clamp(edge_id-1,0,1);	// calculate if vertex is member of right edge

	// modify position towards offset
	pos.x += ofs+wdt*wfac;			// move vertex towards current offset if on right edge
	pos.x += -3*wfac+3*(1-wfac);	// squeezing nanobar edges where splicing
	pos.x *= int(wdt>0);			// killing render when width value not natural
	pos.x += edg_trans[edg_id];

	// calculate final vertex positions
	gl_Position = proj*view*vec4(pos,0,1);
}