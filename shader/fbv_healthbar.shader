#version 330 core

in vec2 position;
in float bar_id;
in float edge_id;

out vec3 coldef;

uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);
uniform float edgediv[5];

void main()
{
	float barlength = edgediv[int(edge_id)];
	gl_Position = proj*view*vec4(position.x+barlength,position.y,0,1);

	coldef = vec3(58,0,84);
}
