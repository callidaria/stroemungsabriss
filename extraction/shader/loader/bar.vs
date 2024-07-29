#version 330 core

in vec2 position;
in vec2 texCoords;

// transformation matrices
uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	gl_Position = proj*view*model*vec4(position,0,1);
}