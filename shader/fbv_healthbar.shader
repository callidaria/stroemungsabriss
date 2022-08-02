#version 330 core

in vec2 position;
in float edge_id;

out vec3 coldef;

uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);
uniform int fill_width;

void main()
{
	gl_Position = proj*view*vec4(position.x+fill_width*edge_id,position.y,0,1);
	coldef = vec3(58,0,84);
}