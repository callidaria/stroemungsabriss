#version 330 core

in vec2 position;
in vec2 dposition;
in vec3 colour;

out vec3 Colour;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

uniform float ptrans = 0.0;

void main()
{
	Colour = colour;
	vec2 tvec = (dposition.xy-position.xy)*ptrans;
	gl_Position = proj*view*model*vec4(position.xy+tvec,0.0,1.0);
}
