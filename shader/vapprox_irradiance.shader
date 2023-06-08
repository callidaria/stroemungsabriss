#version 330 core

in vec3 position;

out vec3 Position;

uniform mat4 proj = mat4(1.0);
uniform mat4 view = mat4(1.0);

void main()
{
	// fragment precalculations or pass through
	Position = position;

	// position output
	gl_Position = (proj*view*vec4(position,1.0)).xyww;
}