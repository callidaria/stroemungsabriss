#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;

out vec4 Position;
out vec2 TexCoords;
out vec4 Normals;
out vec4 light_transpos;

uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);
uniform mat4 light_trans;
uniform int tex_repeat = 1;

void main()
{
	Position=vec4(position.x,position.y,position.z,1.0);
	TexCoords=texCoords*tex_repeat;
	Normals=normalize(vec4(normals.xyz,1.0));
	light_transpos=light_trans*Position;
	gl_Position=proj*view*Position;
}
