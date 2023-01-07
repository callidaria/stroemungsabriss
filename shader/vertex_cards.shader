#version 330 core

in vec3 position;
in vec2 texCoords;

out vec2 TexCoords;

// camera matrices
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

// texture atlas index
uniform vec2 i_tex = vec2(0);

void main()
{
	gl_Position = proj*view*vec4(position,1);
	TexCoords = vec2(texCoords.x/10+i_tex.x/10,texCoords.y/8+i_tex.y/8);
}