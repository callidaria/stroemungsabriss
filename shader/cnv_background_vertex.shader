#version 330 core

in vec2 position;

// camera projection
uniform mat4 view = mat4(1);
uniform mat4 proj = mat4(1);

void main()
{
	gl_Position = proj*view*vec4(position.xy,0,1);
}