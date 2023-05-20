#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;

out vec2 TexCoords;

// camera projection
uniform mat4 proj;
uniform mat4 view;

void main()
{
	// pass through
	TexCoords = texCoords;

	// return position
	gl_Position = proj*view*vec4(position,1.0);
}