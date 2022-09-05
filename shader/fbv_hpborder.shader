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
	// situational variables setup
	float wfac = clamp(edge_id-1,0,1);

	// setup splash-esque geometry
	int upperedge = int(edge_id/2);
	vec2 hfac = vec2(abs(upperedge-1),upperedge);
	vec2 comb = hfac*edg;
	int is_upper = int(edge_id)%2;
	float ccomb = comb.x+comb.y;

	// calculate final vertex positions
	gl_Position = proj*view*vec4((position.x+ofs+wdt*wfac-3*wfac+3*(1-wfac))*int(wdt>0),
			position.y+ccomb*is_upper-ccomb*abs(is_upper-1),0,1);
}