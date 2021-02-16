#version 330 core

in vec2 position;
in vec3 colour;

out vec3 Colour;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	Colour = colour;
	gl_Position = proj*view*model*vec4(position.xy,0.0,1.0);
}
