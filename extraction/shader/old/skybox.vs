#version 330 core

in vec3 position;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 proj;

void main()
{
	TexCoords = position;
	gl_Position = proj*view*vec4(1024*position,1.0);
}
