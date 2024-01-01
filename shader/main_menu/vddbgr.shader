#version 330 core

in vec2 position;

uniform mat4 proj;
uniform mat4 view;

void main()
{
	gl_Position = proj*view*vec4(position,.0,1.);
}
