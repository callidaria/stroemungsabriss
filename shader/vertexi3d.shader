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
out vec3 Normals;
out vec4 light_transpos;
out vec3 TanLightPos;
out vec3 TanViewPos;
out vec3 TanPosition;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);
uniform mat4 light_trans;
uniform int tex_repeat = 1;
uniform vec3 light_pos;
uniform vec3 view_pos;

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
	vec3 rPosition = zrot*yrot*xrot*position;

	// attribute positioning
	TexCoords=texCoords*tex_repeat;
	mat3 norm_mat=mat3(transpose(inverse(model)));
	Normals=normalize(norm_mat*normals);
	Normals=zrot*yrot*xrot*Normals;
	Position=model*vec4(rPosition+offset,1.0);
	gl_Position=proj*view*Position;
	light_transpos=light_trans*Position;

	// normal map precalculations
	vec3 T = normalize(norm_mat*tangent);
	T = normalize(T-dot(T,Normals)*Normals);
	vec3 B = cross(Normals,T);
	mat3 TBN = transpose(mat3(T,B,Normals));
	TanLightPos = TBN*light_pos;
	TanViewPos = TBN*view_pos;
	TanPosition = TBN*Position.xyz;
}