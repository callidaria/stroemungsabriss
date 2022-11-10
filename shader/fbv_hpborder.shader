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
	// situational variables setup
	float wfac = clamp(edge_id-1,0,1);

	// calculate final vertex positions
	gl_Position = proj*view*vec4((position.x+ofs+wdt*wfac-3*wfac+3*(1-wfac))*int(wdt>0)
			+edg_trans[int(edge_id)],position.y,0,1);
}