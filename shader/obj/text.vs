#version 330 core

in vec2 position;
in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj*view*vec4(position,.0,1.);
	TexCoords = texCoords;
}