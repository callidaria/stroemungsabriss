#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;
in vec3 tangent;

out vec4 Position;
out vec2 TexCoords;
out mat3 TBN;

// camera & world transformation
uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

// modifications
uniform float tex_repeat = 1.0;
uniform mat4 light_trans;

void main()
{
	gl_Position = vec4(texCoords,0,1);
	return;

/*
	// calculate position
	Position = model*vec4(position,1.0);
	gl_Position = proj*view*Position;

	// calculate texture coordinates
	TexCoords = texCoords*tex_repeat;

	// reorthogonalize TBN-matrix per-vertex according to gram-schmidt
	vec3 T = normalize((model*vec4(tangent,0)).xyz);
	vec3 Normals = normalize((model*vec4(normals,0)).xyz);
	T = normalize(T-dot(T,Normals)*Normals);
	TBN = mat3(T,cross(Normals,T),Normals);
*/
}