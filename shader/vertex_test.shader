#version 330 core

in vec3 position;
in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 model = mat4(1.0);
uniform mat4 view;
uniform mat4 proj;

void main()
{
	TexCoords = texCoords;
	gl_Position = proj*view*model*vec4(position.xyz,1.0);
}
