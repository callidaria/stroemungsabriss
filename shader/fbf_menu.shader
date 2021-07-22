#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform vec2 fres = vec2(1920,1080); // ??obsolete if calculated pos is uniform
uniform float vgnt = .45;
uniform float ptrans = 0;
uniform sampler2D tex;
uniform sampler2D splash;
uniform sampler2D title;
uniform sampler2D select;

void main()
{
	// texturing
	vec4 proc = texture(tex,TexCoords);
	vec4 sproc = texture(splash,TexCoords);
	vec4 tproc = texture(title,TexCoords);
	vec4 selproc = texture(select,TexCoords); // !!NAMING

	// cinema screen sepia
	vec2 pos = (gl_FragCoord.xy/fres)-vec2(.5);
	float vignette = smoothstep(0.75,0.75-vgnt,length(pos));
	float falpha = min((proc.r+proc.g+proc.b)/1.5,1.0);
	vec3 sepia_proc = vec3(1,.8,0)*(1-falpha)+proc.rgb*falpha;
	sepia_proc = mix(sepia_proc.rgb,sepia_proc.rgb*vignette,.75);
	float alpha = (sepia_proc.r*0.299)+(sepia_proc.g*0.587)+(sepia_proc.b*0.114);
	vec3 sepia = vec3(alpha)*vec3(1.35,1.0,0.65);
	vec4 spproc = vec4(mix(sepia_proc.rgb,sepia,.75),1.0);

	// colour balancing
	vec4 cproc = vec4(sproc.rgb+tproc.rgb+selproc.rgb,1.0);
	outColour = spproc*(1-ptrans)+proc*ptrans+cproc;
	int ltr_balance = int(cproc.r-proc.b>.5); // !!optimize valuation
	int org_balance = int(proc.b>.2);
	outColour = outColour*(1-ltr_balance)+cproc*ltr_balance;
	// !!fix logical horribleness w_o branch
}
