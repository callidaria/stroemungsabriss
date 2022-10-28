#version 330 core

in vec2 position;
in float edge_id;

in float ofs;
in float wdt;
in float dmg;
in vec2 edg;

// camera
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	// result variables
	vec2 pos = position;  // ??what is the meaning of this trash

	// situational variables setup
	float wfac = clamp(edge_id-1,0,1);
	float dmd = (dmg/wdt)*10-5;
	float pofs = ofs+dmg*wfac-dmd*wfac-dmd*wfac+5*(1-wfac);
	pos.x += pofs;
	pos.x *= int(dmg>0);

	// setup splash-esque geometry
	int upperedge = int(edge_id/2);
	vec2 hfac = vec2(abs(upperedge-1),upperedge);
	vec2 comb = hfac*edg;
	int is_upper = int(edge_id)%2;
	float ccomb = comb.x+comb.y;
	ccomb = ccomb*is_upper-ccomb*abs(is_upper-1);
	pos.y += ccomb;

	// calculate final vertex positions
	gl_Position = proj*view*vec4(pos,0,1);
}