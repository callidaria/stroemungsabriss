#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;
in vec3 tangent;

in vec3 offset;
in vec3 rotation_sin;
in vec3 rotation_cos;

out vec4 Position;
out vec2 TexCoords;
out mat3 TBN;

// camera & world transformation
uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

// modifications
uniform float tex_repeat = 1.0;

void main()
{
	// rotate individual mesh duplicates
	mat3 xrot = mat3(
		1,	0,				0,
		0,	rotation_cos.x,	-rotation_sin.x,
		0,	rotation_sin.x,	rotation_cos.x
	);
	mat3 yrot = mat3(
		rotation_cos.y, 0,	-rotation_sin.y,
		0,				1,	0,
		rotation_sin.y,	0,	rotation_cos.y
	);
	mat3 zrot = mat3(
		rotation_cos.z,	-rotation_sin.z,	0,
		rotation_sin.z,	rotation_cos.z,		0,
		0,				0,					1
	);

	// calculate position
	vec3 rPosition = zrot*yrot*xrot*position;
	Position = model*vec4(rPosition+offset,1.0);
	gl_Position = proj*view*Position;

	// calculate texture coordinates
	TexCoords = texCoords*tex_repeat;

	// reorthogonalize TBN-matrix per-vertex according to gram-schmidt
	vec3 Normals = zrot*yrot*xrot*normals;
	vec3 T = normalize((model*vec4(tangent,0)).xyz);
	vec3 N = normalize((model*vec4(Normals,0)).xyz);
	T = normalize(T-dot(T,N)*N);
	TBN = mat3(T,cross(N,T),N);
}