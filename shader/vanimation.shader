#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;
in vec4 boneIndex;
in vec4 boneWeight;

out vec4 Position;
out vec2 TexCoords;
out vec3 Normals;
out vec3 BoneWeight;

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
	BoneWeight = boneWeight.xyz;

	// return position
	gl_Position = proj*view*Position;
}