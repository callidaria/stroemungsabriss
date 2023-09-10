#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;
uniform sampler2D menu_fb;

void main()
{
	// buffer read
	vec4 overlay_tex = texture(tex,TexCoords);
	vec4 menu_tex = texture(menu_fb,TexCoords);

	// merge buffers
	outColour = mix(overlay_tex,vec4(menu_tex.rgb,overlay_tex.a),menu_tex.a);
}