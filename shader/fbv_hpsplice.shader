#version 330 core

in vec2 position;
in float edge_id;

in vec2 ofs[2];

// camera
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	gl_Position = proj*view*vec4(position+ofs[int(edge_id)],0,1);
}