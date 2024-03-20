#version 330 core

in vec2 TexCoords;

out vec4 outColour;

// sepia properties
uniform vec2 fres = vec2(1280,720);  // ??obsolete if calculated pos is uniform
uniform float vgnt = .45;
uniform float ptrans = 0;

// framebuffers
uniform sampler2D tex;

// function definitions
vec4 calculate_sepia(vec4 proc);

void main()
{
	// texturing
	outColour = texture(tex,TexCoords);
	outColour = outColour*ptrans+calculate_sepia(outColour)*(1-ptrans);  // calculate sepia swap
}

vec4 calculate_sepia(vec4 proc)
{
	// calculate vignette darkening
	vec2 pos = (gl_FragCoord.xy/fres)-vec2(.5);
	float vignette = smoothstep(0.75,0.75-vgnt,length(pos));
	float falpha = min((proc.r+proc.g+proc.b)/1.5,1.0);

	// modify pixel alpha
	vec3 sepia_proc = vec3(1,.8,0)*(1-falpha)+proc.rgb*falpha;
	sepia_proc = mix(sepia_proc.rgb,sepia_proc.rgb*vignette,.75);

	// translate into sepia colour range
	float alpha = (sepia_proc.r*0.299)+(sepia_proc.g*0.587)+(sepia_proc.b*0.114);
	vec3 sepia = vec3(alpha)*vec3(1.35,1.0,0.65);

	// mix alpha and and modified colour range and return
	return vec4(mix(sepia_proc.rgb,sepia,.75),1.0);
}