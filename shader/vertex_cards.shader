#version 330 core

// buffer upload
in vec3 position;
in vec2 texCoords;

// index buffer upload
in vec3 tofs;
in vec2 i_tex;

// output to fragment shader
out vec2 TexCoords;

// camera matrices
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	gl_Position = proj*view*vec4(position+tofs,1);
	TexCoords = vec2(texCoords.x/10+i_tex.x/10,texCoords.y/8+i_tex.y/8);
}