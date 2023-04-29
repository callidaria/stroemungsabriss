#version 330 core

in vec3 position;

out vec3 TexCoords;

uniform mat4 proj = mat4(1.0);
uniform mat4 view = mat4(1.0);

void main()
{
	TexCoords = position;
	gl_Position = (proj*view*vec4(position,1.0)).xyww;
}