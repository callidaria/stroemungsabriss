#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;
in vec3 tangent;
in vec3 bitangent;

out vec4 Position;
out vec2 TexCoords;
out mat3 TBN;
out vec3 ltp;

// camera & world transformation
uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

// modifications
uniform float tex_repeat = 1.0;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform mat4 light_trans;

void main()
{
	// calculate position
	Position = model*vec4(position,1.0);
	gl_Position = proj*view*Position;

	// calculate texture coordinates
	TexCoords = texCoords*tex_repeat;

	// reorthogonalize TBN-matrix per-vertex according to gram-schmidt
	vec3 T = normalize((model*vec4(tangent,0)).xyz);
	vec3 N = normalize((model*vec4(normals,0)).xyz);
	T = normalize(T-dot(T,N)*N);
	TBN = mat3(T,cross(N,T),N);

	// calculate light position and transform to screen space
	vec4 rltp = light_trans*Position;
	ltp = (rltp.xyz/rltp.w)*.5+.5;
}