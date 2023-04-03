#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;
in vec3 tangent;
in vec3 bitangent;

out vec4 Position;
out vec2 TexCoords;
out vec3 Normals;

// camera & world transformation
uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

// texture modification
uniform float tex_repeat = 1.0;

void main()
{
	// calculate position
	Position = model*vec4(position,1.0);
	gl_Position = proj*view*Position;

	// calculate texture coordinates
	TexCoords = texCoords*tex_repeat;

	// calculate normals
	mat3 norm_mat = transpose(inverse(mat3(model)));
	Normals = normalize(norm_mat*normals);
}