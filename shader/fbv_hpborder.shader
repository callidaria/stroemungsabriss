#version 330 core

in vec2 position;
in float edge_id;

in float ofs;
in float wdt;

uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	float wfac = clamp(edge_id-1,0,1);
	gl_Position = proj*view*vec4(position.x+ofs+wdt*wfac-2*wfac+2*(1-wfac),position.y,0,1);
}