#version 330 core

in vec2 position;
in float edge_id;

in vec2 ofs[2];
in float spread;

// additional
uniform int cnt_height = 10;

// camera
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	// input conversion
	vec2 pos = position;
	int edg_id = int(edge_id);

	// modify position by spread
	vec2 elevated_upper = vec2(0,cnt_height*edg_id);
	vec2 vrtx_delta = (ofs[1]+elevated_upper)-ofs[0];
	pos += ofs[0]-elevated_upper;
	pos += vrtx_delta*edg_id*spread;

	// return modified vertex positions
	gl_Position = proj*view*vec4(pos,0,1);
}