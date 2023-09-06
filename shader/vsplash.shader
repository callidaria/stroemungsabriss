#version 330 core

in vec2 position;
in float edge_id;

uniform mat4 proj;
uniform mat4 view;

void main()
{
	gl_Position = proj*view*vec4(position.x+100*edge_id,position.y,0,1);
}