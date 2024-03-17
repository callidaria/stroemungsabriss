#version 330 core

in vec2 position;

// selection scrolling
uniform float mv_select = .0;

// coordinate system
uniform mat4 proj;
uniform mat4 view;

void main()
{
	gl_Position = proj*view*vec4(position.x,position.y-mv_select,.0,1.);
}
