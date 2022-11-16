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
	// input variables to writable
	vec2 pos = position;
	int edg_id = int(edge_id);

	// situational variables setup
	float wfac = clamp(edge_id-1,0,1);	// calculate if vertex is member of right edge
	float dmd = (dmg/wdt)*10-5;			// approximate 0 offset when depleted to not self-overlap

	// modify position towards offset
	pos.x += ofs+dmg*wfac;				// move vertex to current offset if on right edge
	pos.x += -dmd*wfac+5*(1-wfac);		// squeezing nanobar edges where splicing
	pos.x *= int(dmg>0);				// killing render when width value not natural

	// interpolate between edge transformations
	int is_upper = int(mod(edg_id,2));
	float delta_trans = edg_trans[is_upper+2]-edg_trans[is_upper];
	pos.x += edg_trans[is_upper]+delta_trans*(dmg/wdt)*wfac;

	// calculate final vertex positions
	gl_Position = proj*view*vec4(pos,0,1);
}