#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;
in vec4 boneIndex;
in vec4 boneWeight;

out vec4 Position;
out vec2 TexCoords;
out vec3 Normals;

// camera projection
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model = mat4(1.0);

// animation
uniform mat4 joint_transform[64];

void main()
{
	// pass through
	TexCoords = texCoords;

	// animation transformation
	mat4 index_transform = mat4(.0);
	for (int i=0;i<4;i++) index_transform += joint_transform[int(boneIndex[i])]*boneWeight[i];
	Position = index_transform*vec4(position,1.0);
	Normals = (index_transform*vec4(normals,1.0)).xyz;

	// return position
	gl_Position = proj*view*model*Position;
}