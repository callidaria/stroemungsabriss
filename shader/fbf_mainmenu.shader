#version 330 core

in vec2 TexCoords;

out vec4 outColour;

// animation
uniform float mtransition = 0;

// sepia
uniform vec2 ratio = vec2(1280,720);
uniform float vignette = .45;

// framebuffer
uniform sampler2D tex;

void main()
{
	// buffer colour
	vec4 pcolour = texture(tex,TexCoords);

	// calculate vignette darkening
	float vignette_final = smoothstep(.75,.75-vignette,length((gl_FragCoord.xy/ratio)-vec2(.5)));
	float falpha = min((pcolour.r+pcolour.g+pcolour.b)/1.5,1.0);

	// modify pixel alpha
	vec3 sepia_proc = vec3(1.,.8,0)*(1.-falpha)+pcolour.rgb*falpha;
	sepia_proc = mix(sepia_proc.rgb,sepia_proc.rgb*vignette_final,.75);

	// translate into sepia colour range
	float alp = (sepia_proc.r*.299)+(sepia_proc.g*.587)+(sepia_proc.b*.114);
	vec3 sepia = vec3(alp)*vec3(1.35,1.,.65);

	// mix sepia transition
	vec4 sepia_final = vec4(mix(sepia_proc.rgb,sepia,.75),1.);
	outColour = mix(sepia_final,pcolour,mtransition);
}