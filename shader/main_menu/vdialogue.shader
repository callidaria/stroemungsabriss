#version 330 core

in vec2 position;
in float disp_id;

// opening transition
uniform vec2 displace[3];
uniform float tprogress;

// coordinate system
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj*view*vec4(position+displace[int(disp_id)]*vec2(tprogress),.0,1.);
}
