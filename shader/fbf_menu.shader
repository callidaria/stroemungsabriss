#version 330 core

in vec2 TexCoords;

out vec4 outColour;

// sepia properties
uniform vec2 fres = vec2(1920,1080); // ??obsolete if calculated pos is uniform
uniform float vgnt = .45;
uniform float ptrans = 0;

// framebuffers
uniform sampler2D tex;
uniform sampler2D splash;

// function definitions
vec4 calculate_sepia(vec4 proc);

void main()
{
	// texturing
	vec4 proc = texture(tex,TexCoords);
	vec4 sproc = texture(splash,TexCoords);

	// effects
	vec4 spproc = calculate_sepia(proc);
	vec4 sep_swap = (spproc*(1-ptrans)+proc*ptrans);  // calculating the sepia swap

	// menu splash
	int splashed = int(sproc.r+.9);  // check if pixel has been splashed
	sep_swap = mix(sep_swap,1.0-proc,splashed);  // invert writing
	sep_swap = mix(sep_swap,proc,proc.b*2*abs(splashed-1));  // reset sepia when blue

	// selection splash inversions
	int tsplash = int(sproc.r+.9);  // if hit by title splash
	int ssplash = int(sproc.g+.9);  // if hit by selection splash
	sep_swap = mix(sep_swap,vec4(0),proc.b*tsplash);  // invert title splash
	sep_swap = mix(sep_swap,vec4(0),proc.r*ssplash);  // invert selection splash
	sep_swap = mix(sep_swap,vec4(/*1.0-proc.rgb*/vec3(1,0,1),1.0),(proc.b-proc.r)*ssplash);

	// render output
	outColour = mix(sep_swap+sproc,sproc,splashed-proc.b*splashed);
	outColour = mix(outColour,outColour+sproc,1-proc.b+tsplash);
	outColour = mix(outColour,sproc,tsplash-proc.b*tsplash);
}

vec4 calculate_sepia(vec4 proc)
{
	vec2 pos = (gl_FragCoord.xy/fres)-vec2(.5);
	float vignette = smoothstep(0.75,0.75-vgnt,length(pos));
	float falpha = min((proc.r+proc.g+proc.b)/1.5,1.0);
	vec3 sepia_proc = vec3(1,.8,0)*(1-falpha)+proc.rgb*falpha;
	sepia_proc = mix(sepia_proc.rgb,sepia_proc.rgb*vignette,.75);
	float alpha = (sepia_proc.r*0.299)+(sepia_proc.g*0.587)+(sepia_proc.b*0.114);
	vec3 sepia = vec3(alpha)*vec3(1.35,1.0,0.65);
	return vec4(mix(sepia_proc.rgb,sepia,.75),1.0);
}