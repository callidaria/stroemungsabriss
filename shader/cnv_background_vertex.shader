#version 330 core

in vec2 position;
in float edge_id;

out vec2 TexCoords;

// edge geometry manipulation
uniform int edge_offset[4];
uniform vec2 tex_coords[4];

// camera projection
uniform mat4 view = mat4(1);
uniform mat4 proj = mat4(1);

void main()
{
	gl_Position = proj*view*vec4(position.x+edge_offset[int(edge_id)],position.y,0,1);
	TexCoords = tex_coords[int(edge_id)];
}