#version 330 core

in vec2 TexCoords;

out vec4 outColour;

// transition
uniform float ctrans = 1.0f;

// scene background
uniform sampler2D scene_tex;

void main()
{
	vec4 tex = texture(scene_tex,TexCoords);
	float bw_switch = int((tex.r+tex.g+tex.b)/3.0+.5);
	vec3 atomic_blend = mix(tex.rgb,vec3(bw_switch),ctrans);
	outColour = vec4(atomic_blend,1.0f);
}