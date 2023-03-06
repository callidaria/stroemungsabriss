#version 330 core

in vec2 position;
in float edge_id;

// edge geometry manipulation
uniform int edge_offset[4];

// camera projection
uniform mat4 view = mat4(1);
uniform mat4 proj = mat4(1);

void main()
{
	gl_Position = proj*view*vec4(position.x+edge_offset[int(edge_id)],position.y,0,1);
}