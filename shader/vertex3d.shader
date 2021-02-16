#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;
in vec3 tangent;
in vec3 bitangent;

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
	TexCoords=texCoords*tex_repeat;
	mat3 norm_mat=mat3(transpose(inverse(model)));
	Normals=normalize(norm_mat*normals);
	Position=model*vec4(position,1.0);
	gl_Position=proj*view*Position;
	light_transpos=light_trans*Position;

	vec3 T = normalize(norm_mat*tangent);
	T = normalize(T-dot(T,Normals)*Normals);
	vec3 B = cross(Normals,T);
	mat3 TBN = transpose(mat3(T,B,Normals));
	TanLightPos = TBN*light_pos;
	TanViewPos = TBN*view_pos;
	TanPosition = TBN*Position.xyz;
}
