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
	// result variables to writable
	vec2 pos = position;

	// situational variables setup
	float wfac = clamp(edge_id-1,0,1);
	float dmd = (dmg/wdt)*10-5;
	float pofs = ofs+dmg*wfac-dmd*wfac+5*(1-wfac);
	pos.x += pofs;
	pos.x *= int(dmg>0);

	// edge transformation
	pos.x += edg_trans[int(edge_id)];

	// calculate final vertex positions
	gl_Position = proj*view*vec4(pos,0,1);
}