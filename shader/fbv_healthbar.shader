#version 330 core

in vec2 position;
in float edge_id;

in float ofs;
in float wdt;
in float dmg;

out vec3 coldef;

uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	float dmd = (dmg/wdt)*10-5;
	gl_Position = proj*view*vec4((position.x+ofs+dmg*edge_id-dmd*edge_id+5*(1-edge_id))*int(dmg>0),
			position.y,0,1);
	coldef = vec3(58,0,84);
}