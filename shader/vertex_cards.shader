#version 330 core

// buffer upload
in vec3 position;
in vec2 texCoords;
in vec3 normals;
in float texID;

// index buffer upload
in vec3 tofs;
in vec2 i_tex;
in vec3 rotation_sin;
in vec3 rotation_cos;
in float deckID;

// output to fragment shader
out vec3 Position;
out vec2 TexCoords;
out vec3 Normals;
out vec3 ltp;

// camera matrices
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

// shadow matrix
uniform mat4 light_trans;

void main()
{
	// rotate individual cards by precalculated sine & cosine values
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
	Position = (zrot*yrot*xrot*position)+tofs;
	Normals = normalize(zrot*yrot*xrot*normals);

	// calculate final vertex position
	gl_Position = proj*view*vec4(Position,1);

	// calculate texture coordinate manipulation on card game atlas
	vec2 tidx = i_tex*(1-texID)+vec2(9,4+deckID)*texID;
	TexCoords = vec2(texCoords.x/10+tidx.x/10,texCoords.y/8+tidx.y/8);

	// precalculate shadow translation for fragment
	vec4 rltp = light_trans*vec4(Position,1);
	ltp = (rltp.xyz/rltp.w)*.5+.5;
}