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
uniform mat4 joint_transform[32];

void main()
{
	// pass through
	TexCoords = texCoords;
	Normals = normals;

	// animation transformation
	Position = vec4(0.0);
	for (int i=0;i<4;i++)
		Position += joint_transform[int(boneIndex[i])]*boneWeight[i]*vec4(position,1.0);

	// return position
	gl_Position = proj*view*model*Position;
}