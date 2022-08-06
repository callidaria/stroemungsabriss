#version 330 core

in vec2 position;
in vec2 offset;

uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	gl_Position = proj*view*vec4(position+offset,0,1);
}