#version 330 core

in vec2 position;
in vec2 texCoords;

out vec2 TexCoords;

// camera attributes
uniform mat4 view;
uniform mat4 proj;

// texture splitting
uniform int col_max;
uniform float col_index;

void main()
{
	gl_Position = proj*view*vec4(position,0,1);
	TexCoords = vec2(texCoords.x/col_max+col_index/col_max,texCoords.y);
}