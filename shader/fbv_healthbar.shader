#version 330 core

in vec2 position;
in float edge_id;

in float ofs;
in float wdt;
in float dmg;
in vec2 edg;

out vec3 coldef;

// camera
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	// situational variables setup
	float wfac = clamp(edge_id-1,0,1);
	float dmd = (dmg/wdt)*10-5;

	// setup splash-esque geometry
	int upperedge = int(edge_id/2);
	vec2 hfac = vec2(abs(upperedge-1),upperedge);
	vec2 comb = hfac*edg;
	int is_upper = int(edge_id)%2;

	// calculate final vertex positions
	gl_Position = proj*view*vec4((position.x+ofs+dmg*wfac-dmd*wfac+5*(1-wfac))*int(dmg>0),
			position.y+(comb.x+comb.y)*is_upper,0,1);

	// experimental colour definition
	coldef = vec3(.5,0,1);
}