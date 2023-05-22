#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;

out vec4 Position;
out vec2 TexCoords;
out vec3 Normals;

// camera projection
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model = mat4(1.0);

void main()
{
	// pass through
	Position = model*vec4(position,1.0);
	TexCoords = texCoords;
	Normals = normals;

	// return position
	gl_Position = proj*view*Position;
}