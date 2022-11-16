#version 330 core

in vec2 position;
in vec2 texCoords;
in vec2 offset;
in vec2 texpos;
in vec2 bounds;
in vec2 cursor;

out vec2 TexCoords;

uniform float wdt = 10.0;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

// TODO: uniform the bounds division floats based on texwidth & texheight
// TODO: uniform norm distance division floats

void main()
{
	// modify texture coordinates based on requested character
	TexCoords = vec2(texCoords.x*(bounds.x/512.0)+texpos.x/512.0,
			texCoords.y*(bounds.y/512.0)+texpos.y/512.0);

	// transform character towards bounds and to cursor position
	vec2 sp = vec2(position.x*(bounds.x/83.0),position.y*(bounds.y/83.0));
	vec2 sc = cursor*(wdt/83.0);

	// assemble final vertex position
	gl_Position = proj*view*model*vec4(sp.x+offset.x-sc.x,sp.y+offset.y-sc.y,0.0,1.0);
}