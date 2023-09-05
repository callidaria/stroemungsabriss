#version 330 core

in vec2 position;
in float edge_id;

void main()
{
	gl_Position = vec4(position.x+100*edge_id,position.y,0,1);
}