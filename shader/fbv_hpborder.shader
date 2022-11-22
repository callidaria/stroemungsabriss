#version 330 core

in vec2 position;
in float edge_id;

in float ofs;
in float wdt;
in float dmg;
in float edg_trans[4];
in vec2 flt;
in float target;

// additional
uniform int cnt_height = 10;
uniform int brd_space = 2;

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

	// vertex directions
	int is_upper = int(mod(edg_id,2));
	vec2 vrtx_dwn = vec2(pos.x+ofs+wdt*wfac+edg_trans[int(edg_id/2)*2],pos.y-cnt_height*is_upper);
	vec2 vrtx_up = vec2(pos.x+ofs+wdt*wfac+edg_trans[int(edg_id/2)*2+1],
			pos.y+cnt_height*(1-is_upper));
	vec2 vrtx_dir = normalize(vrtx_up-vrtx_dwn);

	// directional elongations of vertex elevation
	vrtx_dwn += vrtx_dir*vec2(-brd_space);
	vrtx_up += vrtx_dir*vec2(brd_space);

	// create position based on elevated vertices
	// pos = (vrtx_dwn*(1-is_upper)+vrtx_up*is_upper)*int(wdt>0);
	pos.x += -3*wfac+3*(1-wfac);

	// output final vertex position
	gl_Position = proj*view*vec4(pos+flt,0,1);
}