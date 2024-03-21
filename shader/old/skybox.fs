#version 330 core

in vec3 TexCoords;

out vec4 outColour;

uniform samplerCube skybox;

void main()
{
	outColour = texture(skybox,TexCoords);
}
