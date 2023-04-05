#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;
in vec3 tangent;
in vec3 bitangent;

out vec4 Position;
out vec2 TexCoords;
out mat3 TBN;

// camera & world transformation
uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

// modifications
uniform float tex_repeat = 1.0;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
	// calculate position
	Position = model*vec4(position,1.0);
	gl_Position = proj*view*Position;

	// calculate texture coordinates
	TexCoords = texCoords*tex_repeat;

	// precalculate TBN-matrix for normal manipulation
	TBN = mat3(
		normalize((model*vec4(tangent,0)).xyz),
		normalize((model*vec4(bitangent,0)).xyz),
		normalize((model*vec4(normals,0)).xyz)
	);
}