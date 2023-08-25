#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;
in vec3 tangent;
in vec3 bitangent;
in vec4 boneIndex;
in vec4 boneWeight;

out vec4 Position;
out vec2 TexCoords;
out mat3 TBN;

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
	Position = model*index_transform*vec4(position,1.0);

	// TBN-matrix with gram-schmidt reorthogonalization
	vec3 T = normalize((model*index_transform*vec4(tangent,.0)).xyz);
	vec3 Normals = normalize((model*index_transform*vec4(normals,.0)).xyz);
	T = normalize(T-dot(T,Normals)*Normals);
	TBN = mat3(T,cross(Normals,T),Normals);

	// return position
	gl_Position = proj*view*Position;
}