#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D gbuffer_colour;
uniform sampler2D gbuffer_normals;
uniform sampler2D menu_fb;

// transition
uniform float mtrans = .0;

void main()
{
	// buffer read
	vec4 menu_tex = texture(menu_fb,TexCoords);
	vec4 overlay_tex = texture(gbuffer_colour,TexCoords);
	vec4 overlay_norm = texture(gbuffer_normals,TexCoords);

	// text colour intersection inversion
	vec3 menu_colour = abs(mtrans-menu_tex.rgb);

	// merge buffers
	outColour = mix(overlay_tex,vec4(menu_colour,overlay_tex.a),menu_tex.a);
}
