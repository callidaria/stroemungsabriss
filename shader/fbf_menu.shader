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
uniform sampler2D title;
uniform sampler2D select;
uniform sampler2D cross;

// function definitions
vec4 calculate_sepia(vec4 proc);

void main()
{
	// texturing
	vec4 proc = texture(tex,TexCoords);
	vec4 sproc = texture(splash,TexCoords);
	vec4 tproc = texture(title,TexCoords);
	vec4 selproc = texture(select,TexCoords);
	vec4 crssproc = texture(cross,TexCoords);

	// effects
	vec4 spproc = calculate_sepia(proc);

	// colour balancing
	vec4 sep_swap = (spproc*(1-ptrans)+proc*ptrans);			// calculating the sepia swap
	vec4 splash = vec4(sproc.rgb+tproc.rgb+selproc.rgb+crssproc.rgb,1.0);	// fuse the splashes

	// render
	outColour = sep_swap+splash;
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
