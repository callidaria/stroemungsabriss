#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;

out vec3 Position;
out vec2 TexCoords;
out vec3 Normals;

// camera projection
uniform mat4 proj;
uniform mat4 view;

void main()
{
	// pass through
	Position = position;
	TexCoords = texCoords;
	Normals = normals;

	// return position
	gl_Position = proj*view*vec4(position,1.0);
}